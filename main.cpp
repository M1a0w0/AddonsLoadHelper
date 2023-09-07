#include <iostream>
#include <ctime>
#include <fstream>
#include <filesystem>
#include "jsoncpp/include/json/json.h"

#define zh_cn//lang

/*languages*/
#ifdef zh_cn
    /*zh-cn*/
    #define msg_tips_head "[Add-ons加载工具]使用前请确认:"
    #define msg_tips_0 "\t1.已备份重要数据"
    #define msg_tips_1 "\t2.本程序放置在存档根目录"
    #define msg_tips_2 "\t\t(即同目录下含有文件夹 resource_packs 与 behavior_packs)"
    #define msg_tips_3 "\t3.同级目录下不建议保留 world_behavior_packs.json 与 world_resource_packs.json"
    #define msg_confirm "请确认上述注意事项(Y/y): "
    #define msg_beh_lose "behavior_packs 不存在"
    #define msg_res_lose "resource_packs 不存在"
    #define msg_error "[错误]"
    #define msg_exit "[Add-ons加载工具]程序已退出"
    #define msg_success "[成功]"
    #define msg_readed "已读取文件: "
    #define msg_unread "未读取文件: "
    #define msg_saved "已保存文件: "
#else
    /*en-us*/
    #define msg_tips_head "[AddonsLoadHelper]Please confirm:"
    #define msg_tips_0 "\t1.Backup important data"
    #define msg_tips_1 "\t2.This program is placed in the worlds directory"
    #define msg_tips_2 "\t\t(which folder has resource_packs and behavior_packs)"
    #define msg_tips_3 "\t3.It is not recommended to keep world_behavior_packs.json and world_resource_packs.json in the same level directory"
    #define msg_confirm "Please confirm the above precautions(Y/y): "
    #define msg_beh_lose "behavior_packs losed"
    #define msg_res_lose "resource_packs losed"
    #define msg_error "[Error]"
    #define msg_exit "[AddonsLoadHelper]Program exited"
    #define msg_success "[Success]"
    #define msg_readed "File read: "
    #define msg_unread "Unread file: "
    #define msg_saved "File saved: "
#endif

using namespace std;
namespace fs = std::filesystem;

void printtime();
void get_file(string path, string name, vector<fs::path>& files);
void write_file(string name, vector<fs::path>& files);

int main(){
    char option;
    vector<fs::path> beh_files, res_files;

    cout<<endl<<msg_tips_head<<endl;
    cout<<msg_tips_0<<endl;
    cout<<msg_tips_1<<endl<<msg_tips_2<<endl;
    cout<<msg_tips_3<<endl;
    cout<<msg_confirm;

    option = getchar();
    cout<<endl;
    if(option == 'Y'||option == 'y'){
        if(fs::is_directory("./behavior_packs/")){
            get_file("./behavior_packs/", "manifest", beh_files);
            write_file("world_behavior_packs.json", beh_files);
        }else{
            cout<<msg_error;
            printtime();
            cout<<msg_beh_lose<<endl;
        }
        if(fs::is_directory("./resource_packs/")){
            get_file("./resource_packs/", "manifest", res_files);
            write_file("world_resource_packs.json", res_files);
        }else{
            cout<<msg_error;
            printtime();
            cout<<msg_res_lose<<endl;
        }
    }

    cout<<endl<<msg_exit<<endl;
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
                cout<<msg_success;
                printtime();
                cout<<msg_readed<<files[i]<<endl;
                ifs.close();
            }else{
                throw -1;
            }
        }catch(int){
            cout<<msg_error;
            printtime();
            cout<<msg_unread<<files[i]<<endl;
        }
    }

    ofs.open(name, ofstream::binary);
    try{
        if(ofs.is_open()){
            cout<<msg_success;
            printtime();
            cout<<msg_saved<<name<<endl;
            ofs<<writer.write(beh_packs);
            ofs.close();
        }else{
            throw -1;
        }
    }catch(int){
        cout<<msg_error;
        printtime();
        cout<<msg_unread<<name<<endl;
    }
}