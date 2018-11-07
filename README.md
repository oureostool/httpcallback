# Eos contract call http RESTful api
In eos contract can call http url api. The following step use EOS account ieoscallback to test contract how to call http api in eos contract.

# Intstall example contract httpcallback
```
cleos --wallet-url http://127.0.0.1:6666 -u https://api.eosnewyork.io set contract ieoscallback ./httpcallback/
cleos --wallet-url http://127.0.0.1:6666 -u https://api.eosnewyork.io set account permission ieoscallback active '{"threshold": 1,"keys": [{"key": "EOS8mqP9YabxdqGFqfevrHLAWYqtPe6ySZ67BFiGKdnE34GwCrkTu","weight": 1}],"accounts": [{"permission":{"actor":"httpcallback","permission":"eosio.code"},"weight":1},{"permission":{"actor":"ieoscallback","permission":"eosio.code"},"weight":1}]}' owner -p ieoscallback
```
# Do the abi for test
```
cleos --wallet-url http://127.0.0.1:6666 -u https://api.eosnewyork.io push action ieoscallback httpget '{"url":"https://api.binance.com/api/v3/ticker/price\?symbol\=EOSUSDT"}' -p ieoscallback
cleos --wallet-url http://127.0.0.1:6666 -u https://api.eosnewyork.io push action ieoscallback httppost '{"noop":"noop"}' -p ieoscallback
cleos --wallet-url http://127.0.0.1:6666 -u https://api.eosnewyork.io push action ieoscallback httppostbin '{"noop":"noop"}' -p ieoscallback
cleos -u https://api.eosnewyork.io get table ieoscallback ieoscallback httpresults
```

# Test Net
All of this can test in [Jungle Testnet](http://jungle.cryptolions.io) and [CryptoKylin Testnet](https://www.cryptokylin.io/). For developing convenient we list the testnet API endpoints here.

### Jungle API endpoints
* https://jungle.eosio.cr:443
* http://jungle.cryptolions.io:18888
* http://dev.cryptolions.io:38888

### CryptoKylin API endpoints
* https://api.kylin-testnet.eospacex.com
* http://kylin.fn.eosbixin.com/
* http://api.kylin.eoseco.com

# Price
**0.003 EOS per request**
