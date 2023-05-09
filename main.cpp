
#import "optional"
#import "windows.h"
#import "tlhelp32.h"
#import "feature_test.h"
#import "utils.h"

/*
 * other method? must prog with window?
 * HWND handle = FindWindowA(nullptr,"explorer.exe");
 * DWORD error = GetLastError();
 *
 */
optional<DWORD> find_process_contain(const string &target){
    PROCESSENTRY32 processEntry; processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool first_for = true;BOOL bRet;
    loop{
        if(first_for) {bRet = Process32First(hProcessSnap, &processEntry);first_for= false;}
        else {bRet = Process32Next(hProcessSnap, &processEntry);}
        if(!bRet) break;
        string now_name  = string((char*)processEntry.szExeFile );
        //print(now_name,target);
        if ( now_name.contains(target) )  return processEntry.th32ProcessID;
    }
    return nullopt;
}

optional<DWORD> wait_process(const string & target){
    print("wait_process");
    loop{
        print("wait:",target );
        auto res = find_process_contain(target);
        if(res.has_value()) return res;
        print(typeid( decltype(res) ).name()  );   // canc
        Sleep(1000);
    }
    return nullopt;
}

optional<void *> get_handle(DWORD pid){
    void * handle = OpenProcess(PROCESS_ALL_ACCESS, false,pid);
    if(handle != nullptr) return handle;
    return nullopt;
}

optional<int> find_memory(DWORD pid,void *handle){
    long long buf[10000]={0};
    SIZE_T buf_readsize;
    DWORD64  addr = 0x00007FF7CD314492;
    DWORD rv = ReadProcessMemory(handle,(LPCVOID)addr,buf,10000* sizeof(long long ),&buf_readsize);
    print(rv);
    for(int i=0;i<1000;i++){
        cout << i << " " << buf[i] << endl;
    }
    print(buf_readsize);

    print("end find");
    return nullopt;
}


int main() {

    auto res = wait_process("notepad");
    if(!res.has_value()) return -1;
    DWORD pid = res.value();
    print(pid);
    auto res2 = get_handle(pid);
    if(!res2.has_value()) return -1;
    void * handle_ptr = res2.value();
    auto res3 = find_memory(pid,handle_ptr);

    Sleep(10000);
    return 0;
}
