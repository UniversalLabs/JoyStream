"use strict";

let _ = require('lodash');
var assert = require('assert');

module.exports = TransactionWatcher;

function TransactionWatcher(pool, intercept) {
  this.pool = pool

  // ensure calling pool.forceSync() requests mempool from peers
  this.pool.network.requestMempool = true

  if (intercept) {
    // pool.setFilter is called when pool is loaded, and on connect.
    // when walletdb is opened it will add all its addresses to the filter and call setFilter
    // If walletdb or anyone else calls setFilter at future times, then we need to intercept
    // so we can make sure to add our own elements to the filter (I'm not sure if happens
    // when we run out of the look ahead addresses and filter is updated does bcoin use setFilter or just addFilter?)
    if(typeof this.pool.setFilter != 'function') {
      throw new Error('pool doesn\'t have a setFilter method');
    }

    if(this.pool.setFilter !== this.pool.__proto__.setFilter) {
      throw new Error('cannot intercept setFilter it is not a prototype method');
    }

    // Intercept the setFilter method
    this.pool.setFilter = _.bind(this._setFilterIntercept, this);

    // capture prototype setFilter method bound to the pool
    this._poolSetFilter = _.bind(this.pool.__proto__.setFilter, this.pool);
  }

  // colllection of transaction hashes we are watching
  // mapped to an array of promise resolvers
  this.monitoredTransactions = {};

  this.pool.on('tx', _.bind(this._onTx, this));

  // Reject all promises when pool is shutdown
  this.pool.on('close', _.bind(this.rejectAll, this));
}

TransactionWatcher.prototype.rejectAll = function() {
  _.each(_.keys(this.monitoredTransactions), function(key, resolve_reject){
    resolve_reject.reject('close');
  })

  this.monitoredTransactions = {}
}

TransactionWatcher.prototype._onTx = function(tx) {
  //get hash of transaction
  let hash = tx.hash();

  let resolvers = this.monitoredTransactions[hash];

  if(resolvers) {

    _.each(resolvers, function(resolve_reject){
      resolve_reject.resolve(tx);
    })

    delete this.monitoredTransactions[hash];
  }

};

TransactionWatcher.prototype._setFilterIntercept = function(filter) {
  //add tx's we want to watch to the filter
  _.each(_.keys(this.monitoredTransactions), _.bind(filter.add, filter));

  // call the pool's original setFilter method
  var p = this._poolSetFilter(filter);

  assert(filter == this.pool.spvFilter);

  return p
};

//transaction hash (not reversed little endian txid) hex string or Buffer
//extreme edge case - what if transaction was just mined a few seconds ago! (we have to rescan)
//we don't need to handle this edge case as the average time for a tx to get confirmed
//as likely more than we are willing to wait for it (as a seller) to appear to serve a buyer
TransactionWatcher.prototype.watch = function(hash, timeout) {
  const self = this

  const mempoolRequestDelay = 1000

  //timeout should be greater than delay before getting mempool
  if(timeout > 0 && timeout < mempoolRequestDelay) {
    timeout += mempoolRequestDelay
  }

  let p = new Promise(function(resolve, reject){
    if(timeout > 0) {
      setTimeout(function(){
        reject('timeout')
      }, timeout);
    }

     // Add to set of monitored transactions
    self._addToSet(hash, {
      resolve: resolve,
      reject: reject
    });

  });

  // Update the spv filter
  self.pool.watch(hash)

  // allow time for bloomfilter to be updated
  setTimeout(function() {
    // get the mem pool incase we already missed the tx
    self.pool.forceSync()
  }, mempoolRequestDelay)

  // return a promise which resolves when the tx is seen.
  // rejected when timeout expires, or pool is closed
  return p
};

TransactionWatcher.prototype._addToSet = function(hash, resolve_reject) {
    var transactions = this.monitoredTransactions[hash] || []
    transactions.push(resolve_reject)
    this.monitoredTransactions[hash] = transactions
}