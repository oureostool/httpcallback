#include <eosiolib/eosio.hpp>             
#include <eosiolib/asset.hpp>             
#include <eosiolib/print.hpp>             

#include "hex.hpp"

using namespace eosio;

/**
 * This example illustrate how to call http api in EOS contract.Currently only support http get and post method.
 */
class httpcallback : public eosio::contract {
public:
  using contract::contract;

  httpcallback(account_name self)
  :eosio::contract(self),
   hr(_self, _self)
  {}

  /**
   * abi for http get
   * @param url  The url for http get
   */
  /// @abi action
  void httpget(std::string url) {
    require_auth(_self);

    std::string http_method("GET");
    std::string post_data;
    int16_t post_data_type;
    action(
      eosio::permission_level(_self, N(active)),
      N(httpcallback), N(urlcall),
      std::make_tuple(_self, url, http_method, post_data, post_data_type)
    ).send();

    print_f("httpcallback::httpget~~url=%~~http_method=%~~post_data=%\n", url, http_method, post_data);
  }

  /**
   * abi for http post
   * @param noop This param do nothing 
   */
  /// @abi action
  void httppost(std::string noop) {
    require_auth(_self);

    std::string url("https://api.eosnewyork.io/v1/chain/get_account");
    std::string http_method("POST");
    std::string post_data("{\"account_name\":\"bitfinexeos1\"}");
    int16_t post_data_type = 1;  // post data type is string
    action(
      eosio::permission_level(_self, N(active)),
      N(httpcallback), N(urlcall),
      std::make_tuple(_self, url, http_method, post_data, post_data_type)
    ).send();

    print_f("httpcallback::httppost~~url=%~~http_method=%~~post_data=%\n", url, http_method, post_data);
  }

  /**
   * sometimes http post data must be binary data,httppostbin show you how to do it.
   * @param noop This param do nothing 
   */
  /// @abi action
  void httppostbin(std::string noop) {
    require_auth(_self);

    std::string url("https://api.eosnewyork.io/v1/chain/get_account");
    std::string http_method("POST");
    std::string post_data("{\"account_name\":\"bitfinexeos1\"}");
    int16_t post_data_type = 2; // post data type is hex string, hex string generate by bin2hex

    std::string hex_post_data = bin2hex((const unsigned char *)post_data.c_str(), post_data.size());

    action(
      eosio::permission_level(_self, N(active)),
      N(httpcallback), N(urlcall),
      std::make_tuple(_self, url, http_method, hex_post_data, post_data_type)
    ).send();

    print_f("httpcallback::httppostbin~~url=%~~http_method=%~~post_data=%\n", url, http_method, post_data);
  }

  /**
   * when use httpget/httppost/httppostbin for a http request, urlcallback will return the http result
   * @param url The url httpget/httppost/httppostbin request befor
   * @param result_code  0:success; 1:http request error; 2:content length of http result return greater than 16kb; 3:other error
   * @param httpstatus_code Http standar status code, 200 eg.
   * @param result Result of the http return. Result must use hexs2bin for transfer to binary data.
   */
  /// @abi action
  void urlcallback(std::string url, uint16_t result_code, uint16_t httpstatus_code, std::string result) {
    require_auth(_self);

    if (0 == result_code) {
      if (200 == httpstatus_code) {
        // http call success
        std::string r = hexs2bin(result.c_str(), result.size());
        // wether the r is string or binary is depend on the http url real return
	// if r is string, we print it and write to the table for test
        print_f("httpcallback::urlcallback~~url=%~~result=%~~r=%~~success\n", url, result, r);
        hr.emplace(_self, [&](auto& t){
          t.id = now();
          t.url = url;
          t.result_code = result_code;
          t.httpstatus_code = httpstatus_code;
          t.result = r;
        });

	// if not a string, r is a binary data. the binary data address is (char *)&r[0], binary data length is r.size()
      }
    }

    {
      uint64_t rc = result_code;
      uint64_t hsc = httpstatus_code;
      std::string r = hexs2bin(result.c_str(), result.size());
      print_f("httpcallback::urlcallback~~url=%~~result_code=%~~httpstatus_code=%~~result=%~~r=%\n", url, rc, hsc, result, r);
    }
  }

  // clear table httpresults
  /// @abi action
  void resultclear(std::string noop) {
    require_auth(_self);

    auto i = hr.begin();
    while (i != hr.end()) {
      i = hr.erase(i);
    }
  }

  // @abi table httpresults
  struct httpresult
  {
    uint64_t id;
    std::string url;
    uint16_t result_code;
    uint16_t httpstatus_code;
    std::string result;

    uint64_t primary_key()const { return id; }
  };
  typedef eosio::multi_index<N(httpresults), httpresult> httpresults;
  httpresults hr;
};

EOSIO_ABI(httpcallback, (httpget)(httppost)(httppostbin)(urlcallback)(resultclear))
