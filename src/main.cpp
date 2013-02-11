#include "debug.h"
#include "User.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <pthread.h>
#include <mysql++/mysql++.h>

#include "Adorabot.h"


using namespace std;
namespace po = boost::program_options;

void* startUserThread(void* _user) {
  ((User*)_user)->start();
}

int main() {

  std::string dbHost;
  std::string dbUser;
  std::string dbPass;
  std::string dbDatabase;
  mysqlpp::Connection db(false);
  std::vector<User*> users;

  // Setup options.
  po::options_description desc("Options");
  desc.add_options()
    ("database.host", po::value< std::string >( &dbHost ),
                      "Address and Port (ex. localhost:3306) of the MySQL Server the controller should to connect to." )
    ("database.username", po::value< std::string >( &dbUser ),
                      "Username of the MySQL Server the controller should to connect to." )
    ("database.password", po::value< std::string >( &dbPass ),
                      "Password of the User of the MySQL Server the controller should to connect to." )
    ("database.database", po::value< std::string >( &dbDatabase ),
                      "Name of the Database on the MySQL server the controller should to connect to." );
  // Load setting file.
  po::variables_map vm;
  std::ifstream settings_file( "Adorabot.conf" , std::ifstream::in );
  po::store( po::parse_config_file( settings_file , desc ), vm );
  settings_file.close();
  po::notify( vm );    

  std::cout << "Using database `" << dbDatabase << "` on " << dbUser << ":" << dbPass << "@" << dbHost << std::endl;






  
  if (db.connect(dbDatabase.c_str(), dbHost.c_str(), dbUser.c_str(), dbPass.c_str())) {
    mysqlpp::Query query = db.query("SELECT * FROM `Users` JOIN `Connections` USING(`user_id`) JOIN `Servers` USING(`server_id`) WHERE `active` = 1");
    if (mysqlpp::StoreQueryResult results = query.store()) {
      cout << "Users:" << endl;
      for (size_t i = 0; i < results.num_rows(); i++) {
        cout << '\t' << results[i]["user_id"] << " - " << results[i]["nick"] << '@' << results[i]["ident"] << endl;
        users.push_back(new User((string)results[i]["nick"], (string)results[i]["ident"]));
        users[i]->setUserid(results[i]["user_id"]);
        users[i]->setServer(new Server((string)results[i]["address"], (string)results[i]["port"], (results[i]["ssl"] == "1")));
        users[i]->setBot(new Adorabot());
        pthread_create( users[i]->getThread(), NULL, startUserThread, (void*) users[i]);
        //users[i]->start();
        cout << "\t\tUser Object: " << users[i]->getUserid() << " - " << *(users[i]->getNick()) << "@" << *(users[i]->getIdent()) << endl;

      }
    }
    else {
      cout << "Failed to get User list: " << query.error() << endl;
    }
  }
  else {
    cout << "DB connection failed: " << db.error() << endl;
  }


for(size_t i=0; i<users.size(); i++) {
  pthread_join( *(users[i]->getThread()), NULL);
}



	// User* user = new User(mynick, mynick);
	// Adorabot* myBot = new Adorabot(user);
	// myBot->start(myServerAddress,myServerPort);
	return 0;
}
