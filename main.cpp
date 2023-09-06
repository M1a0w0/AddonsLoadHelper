#include <iostream>
#include <ctime>
#include <fstream>
#include <filesystem>
#include "jsoncpp/include/json/json.h"

using namespace std;
namespace fs = std::filesystem;

void printtime();
void get_file(string path, string name, vector<fs::path>& files);
void write_file(string name, vector<fs::path>& files);

int main(){
    char option;
    vector<fs::path> beh_files, res_files;

    cout<<endl<<"[AddonsLoader]使用前请确认:"<<endl;
    cout<<"\t1.已备份重要数据"<<endl;
    cout<<"\t2.本程序放置在存档根目录"<<endl<<"\t\t(即同目录下含有文件夹 resource_packs 与 behavior_packs)"<<endl;
    cout<<"\t3.同级目录下不建议保留 world_behavior_packs.json 与 world_resource_packs.json"<<endl;
    cout<<"请确认上述注意事项(Y/y): ";

    option = getchar();
    cout<<endl;
    if(option == 'Y'||option == 'y'){
        if(fs::is_directory("./behavior_packs/")){
            get_file("./behavior_packs/", "manifest", beh_files);
            write_file("world_behavior_packs.json", beh_files);
        }else{
            cout<<"[Error]";
            printtime();
            cout<<"behavior_packs 不存在"<<endl;
        }
        if(fs::is_directory("./resource_packs/")){
            get_file("./resource_packs/", "manifest", res_files);
            write_file("world_resource_packs.json", res_files);
        }else{
            cout<<"[Error]";
            printtime();
            cout<<"resource_packs 不存在"<<endl;
        }
    }

    cout<<endl<<"[AddonsLoader]程序已退出"<<endl;
    system("pause");
    return 0;
}

void printtime(){
    time_t nowtime;
    time(&nowtime);
    tm* p = localtime(&nowtime);
    cout<<"["<<p->tm_hour<<":"<<p->tm_min<<":"<<p->tm_sec<<"]";
}

void get_file(string path, string name, vector<fs::path>& files){
    fs::path p = path, s;
    for(auto& entry : fs::directory_iterator(p)){
        if(fs::is_directory(entry)){
            s = entry.path();
            for(auto& entry : fs::directory_iterator(s)){
                if(fs::is_regular_file(entry)){
                    if(entry.path().stem() == name){
                        files.push_back(entry.path());
                    }
                }
            }
        }
    }
}

void write_file(string name, vector<fs::path>& files){
    Json::Value root, obj, beh_packs, res_packs;
    Json::Reader reader;
    Json::StyledWriter writer;
    ifstream ifs;
    ofstream ofs;

    for(int i=0; i<files.size(); i++){
        ifs.open(files[i], fstream::binary);
        try{
            if(ifs.is_open()){
                if(reader.parse(ifs, root)){
                    obj["pack_id"] = root["header"]["uuid"];
                    obj["version"] = root["header"]["version"];
                    beh_packs.append(obj);
                }else{
                    throw -1;
                }
                cout<<"[Success]";
                printtime();
                cout<<"已读取文件: "<<files[i]<<endl;
                ifs.close();
            }else{
                throw -1;
            }
        }catch(int){
            cout<<"[Error]";
            printtime();
            cout<<"未读取文件: "<<files[i]<<endl;
        }
    }

    ofs.open(name, ofstream::binary);
    try{
        if(ofs.is_open()){
            cout<<"[Success]";
            printtime();
            cout<<"已保存文件: "<<name<<endl;
            ofs<<writer.write(beh_packs);
            ofs.close();
        }else{
            throw -1;
        }
    }catch(int){
        cout<<"[Error]";
        printtime();
        cout<<"无法保存文件: "<<name<<endl;
    }
}