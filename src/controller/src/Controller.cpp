
#include "controller/include/Controller.hpp"
#include "Config.hpp"
#include "view/include/addtorrentdialog.h"
#include "controller/include/Exceptions/ListenOnException.hpp"

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>

#include <QObject>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QThread>

#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif Q_MOC_RUN

Controller::Controller(ControllerState state)
    : session(libtorrent::fingerprint("BR"
                                      ,BITSWAPR_VERSION_MAJOR
                                      ,BITSWAPR_VERSION_MINOR
                                      ,0
                                      ,0)
              ,libtorrent::session::add_default_plugins
              ,libtorrent::alert::error_notification
              +libtorrent::alert::tracker_notification
              +libtorrent::alert::debug_notification
              +libtorrent::alert::status_notification
              +libtorrent::alert::progress_notification
              +libtorrent::alert::performance_warning
              +libtorrent::alert::stats_notification)
    , view(this) {

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();

	// Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
	std::vector<char> buffer;
	libtorrent::bencode(std::back_inserter(buffer), state.getLibtorrentSessionSettingsEntry());
	libtorrent::lazy_entry settingsLazyEntry;
	libtorrent::error_code lazyBdecodeEc;
	libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
	session.load_state(settingsLazyEntry);

	// Add DHT routing nodes
	const std::vector<std::pair<std::string, int>> & r = state.getDhtRouters();

	for(std::vector<std::pair<std::string, int>>::const_iterator i = r.begin()
		, end(r.end()); i != end; ++i) {

		// Add router to session
		session.add_dht_router(*i);

		// Add to controller
		//std::pair<std::string, int> router = *i;
		dhtRouters.push_back(*i);
	}

	// Add some sort of check that we actually have some dht routers?

	// Start DHT node
	session.start_dht();

	// Add torrents to session and to controller
    std::vector<libtorrent::add_torrent_params> & p = state.getTorrentParameters();
	
    for(std::vector<libtorrent::add_torrent_params>::iterator i = p.begin()
        , end(p.end()); i != end; ++i)
        addTorrent(*i);

    // Set libtorrent to call processAlert when alert is created
    session.set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

	// Start listening
	boost::system::error_code listenOnErrorCode;
	session.listen_on(state.getPortRange(), listenOnErrorCode);

	// throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);

    // Start timer which calls session.post_torrent_updates at regular intervals
    statusUpdateTimer.setInterval(POST_TORRENT_UPDATES_DELAY);
    QObject::connect(&statusUpdateTimer, SIGNAL(timeout()), this, SLOT(callPostTorrentUpdates()));
    statusUpdateTimer.start();

    // Restore view state
    //view.restoreState();

    // Show view
    view.show();
}

void Controller::callPostTorrentUpdates() {
    session.post_torrent_updates();
}

void Controller::libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr) {

    /*
     * CRITICAL: Do not under any circumstance make a new call to libtorrent in this routine here, since the network
     * thread in libtorrent will be making this call, and a new call will result in a dead lock.
     */

    // Grab alert pointer and release the auto pointer, this way the alert is not automatically
    // deleted when alertAutoPtr goes out of scope.
    const libtorrent::alert * a = alertAutoPtr.release();

    // Tell bitswapr thread to run processAlert later with given alert as argument
    QMetaObject::invokeMethod(this, "processAlert", Q_ARG(const libtorrent::alert*, a));
}

void Controller::processAlert(const libtorrent::alert * a) {

    // Check that alert is not a queued up linger alert corresponding
    // to a recently removed torrent.

    // something()



    // In each case, tell bitswapr thread to run the given method
	if (libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
	{
		std::cout << "metadata_received_alert" << std::endl;

		/*
		// if we have a monitor dir, save the .torrent file we just received in it
		// also, add it to the files map, and remove it from the non_files list
		// to keep the scan dir logic in sync so it's not removed, or added twice
		torrent_handle h = p->handle;
		if (h.is_valid()) {
			if (!ti) ti = h.torrent_file();
			create_torrent ct(*ti);
			entry te = ct.generate();
			std::vector<char> buffer;
			bencode(std::back_inserter(buffer), te);
			std::string filename = ti->name() + "." + to_hex(ti->info_hash().to_string()) + ".torrent";
			filename = combine_path(monitor_dir, filename);
			save_file(filename, buffer);

			files.insert(std::pair<std::string, libtorrent::torrent_handle>(filename, h));
			non_files.erase(h);
		}
		*/
	}
    else if (libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
        processAddTorrentAlert(p);
    else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
		std::cout << "torrent_finished_alert" << std::endl;

		/*
		p->handle.set_max_connections(max_connections_per_torrent / 2);

		// write resume data for the finished torrent
		// the alert handler for save_resume_data_alert
		// will save it to disk
		torrent_handle h = p->handle;
		h.save_resume_data();
		++num_outstanding_resume_data;
		*/
	}
	else if (libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
	{
		std::cout << "save_resume_data_alert" << std::endl;

		/*
		--num_outstanding_resume_data;
		torrent_handle h = p->handle;
		TORRENT_ASSERT(p->resume_data);
		if (p->resume_data)
		{
			std::vector<char> out;
			bencode(std::back_inserter(out), *p->resume_data);
			torrent_status st = h.status(torrent_handle::query_save_path);
			save_file(combine_path(st.save_path, combine_path(".resume", to_hex(st.info_hash.to_string()) + ".resume")), out);
			if (h.is_valid()
				&& non_files.find(h) == non_files.end()
				&& std::find_if(files.begin(), files.end()
					, boost::bind(&handles_t::value_type::second, _1) == h) == files.end())
				ses.remove_torrent(h);
		}
		*/
	}
	else if (libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
	{
		std::cout << "save_resume_data_failed_alert" << std::endl;

		/*
		--num_outstanding_resume_data;
		torrent_handle h = p->handle;
		if (h.is_valid()
			&& non_files.find(h) == non_files.end()
			&& std::find_if(files.begin(), files.end()
				, boost::bind(&handles_t::value_type::second, _1) == h) == files.end())
			ses.remove_torrent(h);
			*/
	}
	else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
	{
		std::cout << "torrent_paused_alert" << std::endl;
		/*
		// write resume data for the finished torrent
		// the alert handler for save_resume_data_alert
		// will save it to disk
		torrent_handle h = p->handle;
		h.save_resume_data();
		++num_outstanding_resume_data;
		*/
	}
	else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
        processStatusUpdateAlert(p);
    else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
        processTorrentRemovedAlert(p);

    // Delete alert
    delete a;
}

// REMOVE AUTO POINTER, PASS ITERATOR ADDRESS OR REF INSTED
std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> Controller::findTorrentParamsFromInfoHash(const libtorrent::sha1_hash & info_hash) {

    // Allocate iterator copy on heap
    std::vector<libtorrent::add_torrent_params>::iterator * i_heap = new std::vector<libtorrent::add_torrent_params>::iterator();

    // Declare auto pointer pointing to iterator on heap
    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> a_ptr(i_heap);

    // Check if vector is empty, in which case we cannot dereference .begin() iterator
    if(!addTorrentParameters.empty()) {

        // Iterate, using iterator on heap, to find match
        *i_heap = addTorrentParameters.begin();
        std::vector<libtorrent::add_torrent_params>::iterator end = addTorrentParameters.end();

        for(; *i_heap != end;(*i_heap)++) {

            libtorrent::add_torrent_params & params = *(*i_heap);

            // Check for match, and return
            if(params.info_hash == info_hash) {
                std::cout << "did we get here?" << std::endl;

                return a_ptr;
            }
        }
    }

    // Free heap iterator and set auto pointer to 0
    a_ptr.reset();

    // Return null auto_ptr
    return a_ptr;
}

bool Controller::loadResumeDataForTorrent(libtorrent::add_torrent_params & params) const {

    // Check that file exists
    QString resumeFile = QString(params.save_path.c_str()) + QDir::separator() + resumeFileNameForTorrent(params);
    QFile file(resumeFile);

    if(file.exists()) {

        // Open file
        if(!file.open(QIODevice::ReadOnly)) {
            std::cerr << "Could not open : " << resumeFile.toStdString().c_str() << std::endl;
            return false;
        }

        // Read entire file
        QByteArray fullFile = file.readAll();

        // Close file
        file.close();

        // Populate resume_data vector
        for(QByteArray::iterator i = fullFile.begin(), end(fullFile.end()); i != end; i++)
            params.resume_data.push_back(*i);

        return true;
    } else
        return false;
}

void Controller::saveResumeDataForAllTorrent() const {

    for(std::vector<libtorrent::add_torrent_params>::const_iterator i = addTorrentParameters.begin(),
        end(addTorrentParameters.end()); i != end;i++)
        saveResumeDataForTorrent(*i);
}

bool Controller::saveResumeDataForTorrent(libtorrent::add_torrent_params const & params) const {

    // Check that save_path still exists, if not create it,
    // because it could have been deleted by user since addTorrent()
    // If you cant create it, then return false
    if(!(QDir().exists(params.save_path.c_str()) || QDir().mkpath(params.save_path.c_str()))) {

        std::cerr << "Could not create save_path: " << params.save_path << std::endl;
        return false;
    }

    // Open file
    QString resumeFile = QString(params.save_path.c_str()) + QDir::separator() + resumeFileNameForTorrent(params);
    QFile file(resumeFile);

    if(!file.open(QIODevice::WriteOnly)) {

        std::cerr << "Could not open : " << resumeFile.toStdString().c_str() << std::endl;
        return false;
    }

    // Write to file
    file.write(&(params.resume_data[0]), params.resume_data.size());

    // Close file
    file.close();

    return true;
}

QString Controller::resumeFileNameForTorrent(libtorrent::add_torrent_params const & params) const {
    return QString((libtorrent::to_hex(params.info_hash.to_string()) + ".resume").c_str());
}

void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    // Find torrent params in addTorrentParameters
    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> a_ptr = findTorrentParamsFromInfoHash(p->info_hash);

    // Did we find a match
    if(a_ptr.get() != 0) {

        // Remove torrent
        addTorrentParameters.erase(*a_ptr.get());

        // Notify view to remove torrent
        view.removeTorrent(p->info_hash);

        std::cerr << "Found match and removed it." << std::endl;

    } else
        std::cerr << "We found no matching torrent for this." << std::endl;
}

void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

    // Name of torrent
    std::string name("N/A");
    int totalSize = 0;
    if(p->params.ti.get() != 0) {
        name = p->params.ti->name();
        totalSize = (p->params.ti)->total_size();
    } else
        name = p->params.name;

    // Check if there was an error
    if (p->error)
        view.addTorrentFailed(name, p->params.info_hash, p->error);
    else {

        // Save handle
        //torrentHandles.push_back(p->handle);

        /*
		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
        */

        // Add torrent to view
        view.addTorrent(p->handle.info_hash(), name, totalSize);
	}
}

void Controller::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
    view.updateTorrentStatus(p->status);
}

void Controller::addTorrentFromTorrentFile(const QString & torrentFile) {

    // Check that torrent file exists
    if(!QFile::exists(torrentFile)) {
        std::cerr << "Torrent file " << torrentFile.toStdString().c_str() << " does not exist." << std::endl;
        return;
    }

    // Setup parameters
    libtorrent::add_torrent_params params;

    // Load torrent file
    libtorrent::error_code ec;
    boost::intrusive_ptr<libtorrent::torrent_info> torrentInfoPointer = new libtorrent::torrent_info(torrentFile.toStdString().c_str(), ec);
    if(ec) {
        std::cout << "Invalid torrent file: " << ec.message().c_str() << std::endl;
        return;
    }

    // Set torrent info in parameters
    params.ti = torrentInfoPointer;

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);

    std::cerr << "was able to create dialog" << std::endl;
    addTorrentDialog->exec(); // <-- starts new event loop, no more libtorrent alerts are processed in mean time.

    // Delete window
    delete addTorrentDialog;
}

void Controller::addTorrentFromMagnetLink(const QString & magnetLink) {

    // Setup parameters
    libtorrent::add_torrent_params params;
    params.url = magnetLink.toStdString();

    // Parse link to get info_hash, so that resume data can be loaded
    libtorrent::error_code ec;
    libtorrent::parse_magnet_uri(magnetLink.toStdString(), params, ec);

    // Exit if link is malformed
    if(ec) {
        std::cout << "Malf|ormed magnet link: " << ec.message().c_str() << std::endl;
        return;
    }

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);
    addTorrentDialog->exec();

    // Delete window resources
    delete addTorrentDialog;
}

void Controller::removeTorrent(const libtorrent::torrent_handle torrentHandle) {

    // Remove from session
    session.remove_torrent(torrentHandle);
}

libtorrent::torrent_handle Controller::getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash) {
    return session.find_torrent(info_hash);
}

// comment out later
libtorrent::session & Controller::getSession() {
    return session;
}

void Controller::addTorrent(libtorrent::add_torrent_params & params) {

    /*
    * If info_hash is not set, we try and set it.
    * This would typically be the case if torrent was added through torrent
    * file rather than magnet link. The primary reason for this constraint is because searching
    * addTorrentParameters is based on info_hashes
    */
    if(params.info_hash.is_all_zeros()) {

        // Is torrent info set, use it
        if(params.ti.get() != 0 && !params.ti->info_hash().is_all_zeros()) {
            libtorrent::sha1_hash info_hash = params.ti->info_hash();
            params.info_hash = info_hash;
        } else {
            // Throw exception in future
            std::cerr << "no valid info_hash set." << std::endl;
            return;
        }
    }

    // Create save_path if it does not exist
    if(!(QDir()).mkpath(params.save_path.c_str())) {

        std::cerr << "Could not create save_path: " << params.save_path << std::endl;
        return;
    }

    // Load resume data if it exists
    loadResumeDataForTorrent(params);

    // Set parameters
    //params.storage_mode = (storage_mode_t)allocation_mode; //  disabled_storage_constructor;
    //params.flags |= add_torrent_params::flag_paused; //  |= add_torrent_params::flag_seed_mode;
    //params.flags &= ~add_torrent_params::flag_duplicate_is_error;
    //params.flags |= add_torrent_params::flag_auto_managed; // |= add_torrent_params::flag_share_mode;
    //params.userdata = (void*)strdup(torrent.c_str());

	// Add to libtorrent session
    /*
    libtorrent::add_torrent_params d;
    d.save_path = std::string("C:\\");
    d.url = std::string("magnet:?xt=urn:btih:781ad3adbd9b81b64e4c530712ae9199b1dfbae5&dn=Now+You+See+Me+%282013%29+1080p+EXTENDED+BrRip+x264+-+YIFY&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3");
    session.async_add_torrent(d);
    */
    session.async_add_torrent(params);

    // Add to controller
    // - DO WE EVEN NEED TO KEEP TRACK OF THIS? -
    addTorrentParameters.push_back(params);
}

void Controller::saveStateToFile(const char * file) {

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
	session.save_state(libtorrentSessionSettingsEntry);

	// Create state object
	ControllerState controllerState(libtorrentSessionSettingsEntry, 
									portRange, 
									addTorrentParameters, 
									dhtRouters);

	// Save to file
	controllerState.saveToFile(file);
}

void Controller::close() {

    // Pause all torrents
    session.pause();

    // Save all resume data
    saveResumeDataForAllTorrent();

    // Save state of controller (includes full libtorrent state) to parameter file
    QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
    this->saveStateToFile(file.toStdString().c_str());

    // Exit Qt event loop
    QThread::currentThread()->exit();

    // Save state of window
    //view.saveState();
}
