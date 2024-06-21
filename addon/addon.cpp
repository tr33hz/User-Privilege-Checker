#include <napi.h>
#include <windows.h>
#include <lm.h>
#include <string>

#pragma comment(lib, "netapi32.lib")

Napi::String GetUserPrivilege(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    std::string username = info[0].As<Napi::String>().Utf8Value();

    USER_INFO_1 *userInfo = nullptr;
    DWORD dwLevel = 1;
    DWORD dwError = 0;

    std::wstring wusername(username.begin(), username.end());
    NET_API_STATUS nStatus = NetUserGetInfo(NULL, wusername.c_str(), dwLevel, (LPBYTE*)&userInfo);

    if (nStatus == NERR_Success) {
        std::string privilege;
        switch (userInfo->usri1_priv) {
            case USER_PRIV_GUEST:
                privilege = "Гость";
                break;
            case USER_PRIV_USER:
                privilege = "Пользователь";
                break;
            case USER_PRIV_ADMIN:
                privilege = "Администратор";
                break;
            default:
                privilege = "Аноним";
                break;
        }
        NetApiBufferFree(userInfo);
        return Napi::String::New(env, privilege);
    } else {
        return Napi::String::New(env, "Пользователя нет");
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "getUserPrivilege"), Napi::Function::New(env, GetUserPrivilege));
    return exports;
}

NODE_API_MODULE(addon, Init)