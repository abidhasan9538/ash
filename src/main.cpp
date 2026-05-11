#include <iostream>
#include<sstream>
#include <string>
#include<ranges>
#include<unistd.h>
#include<vector>
#include<sys/wait.h>
using namespace std;

int main() {
  
  cout << unitbuf;
  cerr << unitbuf;

  string line;
  string command;
  while(true){
    cout << "$ ";
    getline(cin, line);
    stringstream ss(line);
    ss >> command;
    if(command == "exit"){
      break;
    }
    else if(command == "echo"){
      string word;
      while(ss>>word){
        cout << word << " ";
      }
      cout << endl;
    }
    else if(command == "type"){
      string command_to_know;
      ss>>command_to_know;
      bool found = false;
      string builtin[3] = {"echo", "type", "exit"};
      for(int i = 0; i<3; i++){
        if(command_to_know == builtin[i]){
          cout << command_to_know << " is a shell builtin" << endl;
          found = true;
          break;
        }
      }
      if(!found){
        string path_env = getenv("PATH");
        stringstream ss_path(path_env);
        string path;
        while(getline(ss_path, path,':' )){
          string full_path = path + '/' + command_to_know;
          if(access(full_path.c_str(), X_OK) == 0){
            cout << command_to_know << " is " << full_path << endl;
            found = true;
            break;
          }
        }
      }
      if(!found) cout << command_to_know << ": not found\n";

    }
    else {
    std::vector<std::string> args;
    args.push_back(command);

    std::string word;

    while (ss >> word) {
        args.push_back(word);
    }

    std::string executable = "";

    std::string path_env = getenv("PATH");
    std::stringstream path_stream(path_env);
    std::string path;

    while (getline(path_stream, path, ':')) {
        std::string full_path = path + "/" + command;

        if (access(full_path.c_str(), X_OK) == 0) {
            executable = full_path;
            break;
        }
    }

    if (executable == "") {
        std::cout << command << ": command not found\n";
        continue;
    }

    pid_t pid = fork();

    if (pid == 0) {
        std::vector<char*> c_args;

        for (auto &arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }

        c_args.push_back(nullptr);

        execvp(executable.c_str(), c_args.data());

        std::cout << "Execution failed\n";
    }
    else {
        wait(NULL);
    }
}

  }
  return 0;
}

