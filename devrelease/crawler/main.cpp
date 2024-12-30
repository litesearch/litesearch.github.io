/*
 * Crawler of LiteSearch Written in C++ purely by ghgltggamer
 * LibCurl in Linux needed
 */

// Src
// C++ Local Libs
#include "liblitecrawler.hpp" // An implementation for Crawling methods

// C++ STL
#include <iostream>
#include <fstream>

// Main program
int main(int argc, char* argv[]){
    if (argc == 1){
        std::cout<<"Lite Crawler needs 1 argument at the least\n";
        std::exit (3);
    }
    // Make argment vector
    std::vector <std::string> argument_Vector = {};
    for (int i = 1;i < argc;i++){
        std::cout<<"Adding the domain <"<<argv[i]<<"> to the quee(Argument List)...\n";
        argument_Vector.push_back(argv[i]);
        std::cout<<"done\n";
    }
    // Dataset checking
    std::cout<<"\n\nChecking For Dataset Existance...\n";
    std::ifstream dataset_litds_ifile("dataset.litds");
    std::vector <std::string> dataset_litds = {};
    std::string dataset_litds_file_content = "";
    if (dataset_litds_ifile.is_open()){
        std::cout<<"Dataset found.\nChecking the dataset validity...\n";
        std::string temp;
        bool can_fetch = false, is_valid_dataset = false;
        std::string validity = "";
        while (std::getline(dataset_litds_ifile, temp)){
            validity += temp + "\n";
            if (can_fetch) dataset_litds.push_back(temp);
            if (temp.find("[domains]") != std::string::npos) can_fetch = true;
            if (temp.find("[domains_terminate]") != std::string::npos) can_fetch = false;
        }
        if (validity.find("[domains]") != std::string::npos and temp.find("[domains_terminate]") != std::string::npos) is_valid_dataset = true;
        std::cout<<"Checking done";
        if (is_valid_dataset) {
            std::cout<<" with a valid dataset\n";
            dataset_litds_file_content = validity;
        }
        else {
            std::cout<<" with an invalid dataset\n\n";
            while (true){
                std::cout<<"Would you like to create a new dataset ? (y/n) ";
                char inp;
                std::cin>>inp;
                if (inp == 'n'){
                    std::cout<<"Exiting with invalid dataset, Status Faliure due to invalid dataset.\n";
                    std::exit (3);
                }
                else if (inp == 'y'){
                    std::cout<<"Creating a new blank dataset\n";
                    std::ofstream dataset_litds_ofile("dataset.litds");
                    dataset_litds_ofile<<"[domains]\n[domains_terminate]";
                    dataset_litds_file_content = "[domains]\n[domains_terminate]";
                    std::cout<<"A new dataset was created and now continuing the crawling...\n";
                    break;
                }
                else {
                    std::cout<<"Expected only 'y' or 'n' where 'y' represetns yes and 'n' represents no, Retry please.\n";
                    continue;
                }
            }
        }
    }

    // Start crawling by index
    for (size_t i = 0;i < argument_Vector.size();i++){
        std::cout<<"Starting crawling for <"<<argument_Vector[i]<<">\n";
        std::cout<<"Checking if domain was already registered in the dataset...\n";
        bool can_crawl = true;
        for (size_t x = 0;x < dataset_litds.size();x++){
            if (dataset_litds[x].find(argument_Vector[i]) != std::string::npos){
                std::cout<<"The domain <"<<argument_Vector[i]<<"> was already registered in the dataset. ";
                while (true){
                    std::cout<<"Do you want to Re-Crawl the domain? Note that Re-Crawling will not affect the changes to dataset as domain will be same only frontend dataset will be affected, So would you like to Re-Crawl ? (y/n) ";
                    char conf;
                    std::cin>>conf;
                    if (conf == 'n'){
                        std::cout<<"Not Re-Crawling the domain and continuing for next domain...\n";
                        can_crawl = false;
                        break;
                    }
                    else if (conf == 'y'){
                        std::cout<<"Re-Crawling the domain name <"<<argument_Vector[i]<<">...\n";
                        // Crawling machenasim will go here... Currently this is a placeholder but will be implemented later
                        can_crawl = false;
                        break;
                    }
                    else {
                        std::cout<<"Expected only 'y' or 'n' where 'y' represetns yes and 'n' represents no, Retry please.\n";
                        continue;
                    }
                }
                break;
            }
            else continue;
        }

        // Checking if crawling is allowed or not
        if (can_crawl){
            std::cout<<"Crawling the url <"<<argument_Vector[i]<<" as domain...\nRegistering the url in dataset...\n";
            // Crawling machenasim will go here... Currently this is a placeholder but will be implemented later
            size_t pos_of_domains_in_dataset = dataset_litds_file_content.find("[domains]");
            dataset_litds_file_content.insert(pos_of_domains_in_dataset+std::string("[domains]").length(), std::string("\n"+argument_Vector[i]));
            std::cout<<"done the regestration. Continuing for next...\n";
            dataset_litds_file_content = dataset_litds_file_content.substr(0, dataset_litds_file_content.find("[domains_terminate]"));
            dataset_litds_file_content+="[domains_terminate]";
            std::ofstream new_dataset("dataset.litds");
            new_dataset<<dataset_litds_file_content;
        }
        else continue;
    }
}

// Written by ghgltggamer