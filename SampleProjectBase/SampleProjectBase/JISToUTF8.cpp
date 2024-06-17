#include "JISToUTF8.h"

std::string ShiftJisToUtf8(std::string const& _jisString)
{
    auto const wide = StringToWString(_jisString);
    return WStringToString(wide);
}

std::wstring StringToWString(std::string const& _string)
{
    // SJIS �� wstring
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, _string.c_str()
        , -1, (wchar_t*)NULL, 0);

    // �o�b�t�@�̎擾
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS �� wstring
    MultiByteToWideChar(CP_ACP, 0, _string.c_str(), -1, cpUCS2
        , iBufferSize);

    // string�̐���
    std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

    // �o�b�t�@�̔j��
    delete[] cpUCS2;

    // �ϊ����ʂ�Ԃ�
    return(oRet);
}

std::string WStringToString(std::wstring const& _wstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(_wstring);
}

