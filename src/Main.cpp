#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include <stack>

using asio::ip::tcp;

class httpPacket {
public:
	std::vector<std::pair<std::string, std::string>> variables;
	std::string url;
	std::string content;
	int length;

	void variablesToContent() {
		content = "";
		for (int i = 0;i < variables.size();i++) {
			content += "\" <br> " + variables[i].first + "\" \r\n\r\n" + variables[i].second + "\r\n ------ ";
		}
		length = content.length();
	}
};


std::string CreateHttpGET(std::string url) {
	std::string	s = "GET /"+url+" HTTP/1.1\r\n"+
						"Host: 127.0.0.1:8080\r\n" +
						"Connection: keep-alive\r\n" +
						"Upgrade-Insecure-Requests: 1\r\n" +
						"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029 Safari/537.36 \r\n" +
						"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8	\r\n"+
						"Accept-Encoding: gzip, deflate, sdch, br \r\n"+
						"Accept-Language: en-US,en;q=0.8";
	return s;
}

std::string CreateHttpPost(httpPacket& packet) {
	std::string s = "POST /"+ packet.url+" HTTP / 1.1\r\n"+
					"Host: 127.0.0.1:8080\r\n"+
					"Connection: keep-alive\r\n"+
					"Content-Length: "+std::to_string(packet.length)+"\r\n"+
					"Accept: text/plain, */*; q=0.01\r\n"+
					"Origin: http://127.0.0.1:8080\r\n"+
					"X-Requested-With: XMLHttpRequest\r\n"+
					"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n"+
					"Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n"+
					//"Referer: http://127.0.0.1:8080/Equaton.htm \r\n"+
					"Accept-Encoding: gzip, deflate, br\r\n"+
					"Accept-Language: en-US,en;q=0.8\r\n"+
					"\r\n ------ "+packet.content;
	return s;
}

int reads = 0;

class http_client
{
public:

	http_client(asio::io_service& io_service,tcp::resolver::iterator endpoint_iterator): io_service_(io_service),socket_(io_service_)
	{
		//works
		//io_service_.run();
		do_connect(endpoint_iterator);
		//io_service_.run();

	}

	void close()
	{
		//io_service_.post([this]() { socket_.close(); });
	}
	void loopT() {
		while (true) {
			if (messageStack.size() > 0) {
				do_write(messageStack.top());
				messageStack.pop();
			}
		}
	}
	void ioServiceRUn() {
		io_service_.run();
	}
private:
	
	

	void do_connect(tcp::resolver::iterator endpoint_iterator)
	{
		//works
		asio::connect(socket_, endpoint_iterator);
		//std::cout << socket_.is_open()<<std::endl;


		//asio::async_connect(socket_, endpoint_iterator,
		//	[this](std::error_code ec, tcp::resolver::iterator)
		//{
		//	std::cout << ec.message() << std::endl;
		//	if (!ec)
		//	{
		//		//std::cout << socket_.is_open() << std::endl;
		//		//std::thread loop(&http_client::loopT,this);
		//		/*httpPacket p;
		//		p.url = "Equation";
		//		p.variables.push_back(std::make_pair("aaa", "10+20"));
		//		p.variables.push_back(std::make_pair("abba", "12+30"));
		//		p.variables.push_back(std::make_pair("acccaa", "10222+20"));
		//		p.variablesToContent();
		//		do_write(CreateHttpPost(p));*/
		//		//do_read_body();
		//		std::cout << socket_.is_open() << std::endl;
		//	}
		//});
		//io_service_.run();
	}

	void do_read_body()
	{
		//works
		char data[4096];
		socket_.read_some(asio::buffer(data, 4096));
		reads++;
		std::cout << data << std::endl;
		std::cout << reads << std::endl;

		//char data[4096];
		////failsafe probably useless
		//bool b = socket_.is_open();
		//while (b == 0);

		//asio::async_read(socket_, asio::buffer(data, 4096),
		//	[&data](std::error_code ec, std::size_t /*length*/)
		//{
		//	std::cout << data << std::endl;

		//});
	}
public:
	void do_write(std::string& msg)
	{
		//works
		socket_.write_some(asio::buffer(msg.c_str(), msg.length()));
		do_read_body();


		//bool b =socket_.is_open();
		//while (b == 0);
		//asio::async_write(socket_,
		//	asio::buffer(msg.c_str(),
		//		msg.length()),
		//	[this](std::error_code ec, std::size_t /*length*/)
		//{
		//	std::cout << ec;
		//	if (!ec)
		//	{
		//		do_read_body();
		//	}
		//	else
		//	{
		//		socket_.close();
		//	}
		//});
	}
	std::stack<std::string> messageStack;

private:
	asio::io_service& io_service_;
	tcp::socket socket_;
	char data_[4096];
};


//void do_read(tcp::socket& socket) {
//	char data[4096];
//	socket.read_some(asio::buffer(data, 4096));
//	std::cout << data << std::endl;
//}
//void do_writet(tcp::socket& socket, std::string msg) {
//	socket.write_some( asio::buffer(msg.c_str(), msg.length()));
//}
//void do_connect(tcp::socket& socket,tcp::resolver::iterator iterator) {
//
//	asio::connect(socket, iterator);
//}

int main(int argc, char* argv[])
{
	try
	{

		asio::io_service io_service;
		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ "localhost", "8080" });
		//http_client c(io_service, endpoint_iterator);

		//tcp::socket socket(io_service);
		
		//
		//std::this_thread::sleep_for(std::chrono::seconds(2));

		//std::string s = CreateHttpGET("");

		
		httpPacket p;
		p.url = "Equation";
		p.variables.push_back(std::make_pair("set(x)", "30.324"));
		//p.variables.push_back(std::make_pair("abba", "x+30"));

		/*p.variables.push_back(std::make_pair("acccaa", "10222+20+20*300"));
		p.variables.push_back(std::make_pair("zzzz", "12+30"));
		p.variables.push_back(std::make_pair("zzzasasz", "12+30"));
		p.variables.push_back(std::make_pair("zz12222222222zz", "121212+30"));
	*/	
		//c.do_write(CreateHttpPost(p));
		//io_service.run();

		/*do_connect(socket, endpoint_iterator);
		do_writet(socket, CreateHttpPost(p));
		do_read(socket);*/

		//c.close();
		//io_service.run();

		/*http_client c(io_service, endpoint_iterator);
		http_client a(io_service, endpoint_iterator);
		std::thread t1(&http_client::do_write,&c, CreateHttpPost(p));
		std::thread t2(&http_client::do_write,&a, CreateHttpPost(p));
		t1.join();
		t2.join();*/



		std::vector<std::thread> threadList;
		for (int i = 0; i < 100; i++)
		{
			threadList.push_back(std::thread([&p,i,&io_service,&endpoint_iterator] () {
				//std::cout << i << std::endl;
				//p.variables[5] = std::make_pair("nr", std::to_string(i));
				p.variablesToContent();
				http_client a(io_service, endpoint_iterator);
				
				a.do_write(CreateHttpPost(p));
				p.variables[0] = std::make_pair("nr", "x+20.59");
				p.variablesToContent();
				a.do_write(CreateHttpPost(p));
				/*
				p.variables[5] = std::make_pair("nraa", std::to_string(i));
				a.do_write(CreateHttpPost(p));
				
				a.do_write(CreateHttpGET(""));*/


			}));
		}
		// Now wait for all the worker thread to finish i.e.
		// Call join() function on each of the std::thread object
		std::cout << "wait for all the worker thread to finish" << std::endl;
		std::for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));
		std::cout << "Exiting from Main Thread" << std::endl;



		//c.do_write(CreateHttpPost(p));
		
		//c.messageStack.push(CreateHttpPost(p));

		//socket.close();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	

	return 0;
}
