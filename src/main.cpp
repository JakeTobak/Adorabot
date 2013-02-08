#include "Adorabot.h"
#include "User.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
namespace po = boost::program_options;

int main() {

  std::string mynick;
  std::string myServerAddress;
  std::string myServerPort;

  // Setup options.
  po::options_description desc("Options");
  desc.add_options()
    ("nick", po::value< std::string >( &mynick ),
                      "Nick that the bot should try and use." )
    ("server.address", po::value< std::string >( &myServerAddress ),
                      "Address of the IRC Server to connect to." )
    ("server.port", po::value< std::string >( &myServerPort ),
                      "Port of the IRC Server to connect to." );
  // Load setting file.
  po::variables_map vm;
  std::ifstream settings_file( "Adorabot.conf" , std::ifstream::in );
  po::store( po::parse_config_file( settings_file , desc ), vm );
  settings_file.close();
  po::notify( vm );    

  std::cout << "nick: " << mynick << std::endl;
  std::cout << "server: " << myServerAddress << ":" << myServerPort << std::endl;

	User* user = new User(mynick, mynick);
	Adorabot* myBot = new Adorabot(user);
	myBot->start(myServerAddress,myServerPort);
	return 0;
}
