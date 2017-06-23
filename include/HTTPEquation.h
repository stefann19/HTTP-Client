#pragma once
#include <string>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <map>
#include <ctype.h>
#include "json11.hpp"
#include "asio.hpp"


typedef std::pair<std::string, std::string> stringPair;
typedef std::pair<std::string, std::string>(*ScriptFunction)(std::string); // function pointer type 
typedef std::map<stringPair, ScriptFunction> script_map;

using asio::ip::tcp;


class PacketInfo {
public:
	std::string type;
	std::string httpV;
	std::string host;
	std::string function;
	std::string responseCode;
	std::vector<stringPair> packetBody;
	std::string procceseedPacketBodyString;

	
};

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket);
	char buffsize[4096];
	asio::mutable_buffers_1 buff = asio::buffer(buffsize, 4096);
	void start();


private:
	void do_read();

	void do_write(int length);


	tcp::socket socket_;
	enum { max_length = 4096 };
	char data_[max_length];
};
void Write(PacketInfo packet);


void StartHTTPServicee(int port);