
#import "optional"
#import "windows.h"
#import "tlhelp32.h"
#import "feature_test.h"
#import "utils.h"
#import "string"

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
        if ( now_name.find(target) != now_name.npos )  return processEntry.th32ProcessID;
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
    print("get_handle");
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

optional<int> load_dll(DWORD pid,void *handle){
    print("load_dll");
    DWORD buf_size=1000;
    LPVOID res_ptr = VirtualAllocEx(handle, nullptr,buf_size,MEM_COMMIT,PAGE_READWRITE);
    print("VirtualAllocEx res:",res_ptr);
    string dll_pathname = "kernel32.dll";
    //string dll_pathname = "test_dll.dll";
    if(res_ptr == nullptr) return -1;
    BOOL res = WriteProcessMemory(handle,res_ptr,dll_pathname.c_str(),buf_size, nullptr);
    print("WriteProcessMemory res:",res);
    if(!res){
        //VirtualFreeEx(handle,res_ptr,0,MEM_RELEASE);
        CloseHandle(handle);
        return -2;
    }
    HMODULE hm = LoadLibraryA(dll_pathname.c_str());
    if (hm == nullptr) return -3;
    print(  "TCHAR size:",sizeof(TCHAR)  );
    auto thread_proc = (LPTHREAD_START_ROUTINE)GetProcAddress(hm,"LoadLibraryA");
    print("GetProcAddress res:",thread_proc);
    if (thread_proc== nullptr){
        //VirtualFreeEx(handle,res_ptr,0,MEM_RELEASE);
        CloseHandle(handle);
        return -4;
    }
    HANDLE thread_handle = CreateRemoteThread(handle, nullptr, 0, thread_proc, res_ptr, 0, nullptr);
    print("CreateRemoteThread res:",thread_handle);
    if (thread_handle== nullptr){
        //VirtualFreeEx(handle,res_ptr,0,MEM_RELEASE);
        CloseHandle(handle);
        return -5;
    }
    return 0;
}

int main() {

    auto res = wait_process("otepad"); //notepad
    if(!res.has_value()) return -1;
    DWORD pid = res.value();
    print(pid);
    auto res2 = get_handle(pid);
    if(!res2.has_value()) return -1;
    void * handle_ptr = res2.value();
    auto res3 = load_dll(pid,handle_ptr);

    Sleep(10000);
    return 0;
}
