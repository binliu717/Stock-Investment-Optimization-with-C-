#include<iostream>
#include<vector>
#include<fstream>
#include <cmath>
#include <curl/curl.h>
using namespace std;
typedef long int myint;

double strWord(int index, string line)
{
    int count = 0;
    string word;
    for (int i = 0 ; i < line.length(); i++) {
        if (line[i] == ',') {
            if (line[i+1] != ',') {
                count++;
                if (count == index) {
                    double w=::atof(word.c_str());
                    return w;
                }
                word ="";
            }
        }
        else {
           word += line[i];
        }
    }
}

void getVector(int index, vector<double> &price, string filename){
  ifstream f;
  f.open(filename);
  string line;
  if(f.is_open()){
  while (!f.eof()){
    getline(f,line);
    price.push_back(strWord(index,line));
    }
  }
  f.close();
  price.erase(price.begin());
}

void logReturn(vector<double>& a, vector<double>& b, vector<double>& c, const int& column){
  // this function will calculate the log return of every day per stock
  for(int i=0;i<column;++i){
    c[i]=log(a[i]/b[i]);
  }
}

void getVectorOfReturn(vector<vector<double>>& returns, vector<string>& tickers,
     const int& size, const int& column){

  for(int i=0; i<size; i++){
   vector<double> endPrice;
   vector<double> openPrice;
   vector<double> logReturn;
   endPrice.reserve(column);openPrice.reserve(column);logReturn.reserve(column);
   string filename="quotes"+tickers[i]+".txt";
   getVector(5,endPrice,filename);
   getVector(2,openPrice,filename);

   for(int i=0;i<size;++i){
     logReturn.push_back(endPrice[i]/openPrice[i]-1);
   }
   returns.push_back(logReturn);
  }
}

void tickerName(vector<string>& tickers, const int& size){
  // this function will open up the file named tickers.txt and put all of the tickernames
  // within a vector
  ifstream t;
  string input;
  t.open("tickers.txt");
  string ticker;
  if(t.is_open()){
    for(int i=0;i<size;++i){
      getline(t,ticker);
      tickers.push_back(ticker);
     }
    }
  t.close();
}

typedef long int myint;
void printToFile(std::string filename, const std::string &text){
  std::ofstream mfile;
  mfile.open(filename);
  for(myint i=0;i<text.length();++i){
  mfile<<text[i];}
  mfile.close();
}
static size_t writerF(void *ptr, size_t size, size_t nmemb, void *userdata){
  ((std::string*)userdata)->append((char*)ptr, size * nmemb);
  return size * nmemb;
}
void stockDataToFile(const std::string &tickerName,
const std::string &quandl_auth_token,
const std::string &database="WIKI",
const std::string &folder="quotes"){
  std::string mainLink="https://www.quandl.com/api/v3/datasets/";
  mainLink+=database;
  mainLink+="/"+tickerName;
  mainLink+=".csv";
  mainLink+="?sort_order=asc&auth_token=";
  mainLink+=quandl_auth_token;
  CURL *curl;
  std::string quandlData;
  std::string fName=folder;
  fName+=tickerName;
  fName+=".txt";
  curl = curl_easy_init();
  if(curl) {
  const char* linkArrayChar=mainLink.c_str();
  curl_easy_setopt(curl, CURLOPT_URL, linkArrayChar);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerF);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &quandlData);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  printToFile(fName,quandlData);
 }
}


void printToFile(string filename, const string &text){
  ofstream mfile;
  mfile.open(filename);
  for(myint i=0;i<text.length();++i){
    mfile<<text[i];
  }
  mfile.close();
}


static size_t writerF(void *ptr, size_t size, size_t nmemb, void *userdata){
  ((string*)userdata)->append((char*)ptr, size * nmemb);
  return size * nmemb;
}