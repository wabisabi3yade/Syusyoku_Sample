#pragma once

// �Q�l�T�C�g  https://qiita.com/Chironian/items/643d2e1a25d4f258f3e4

template<typename tType>
class TypeName
{

    template<typename tType>
    static void GetTypeNameImpl(std::string& _name)
    {
        _name = typeid(tType).name();
        size_t pos = _name.find(" ");
        if (pos != std::string::npos)
        {
            _name = _name.erase(0, pos + 1);
        }
    }

public:
    static std::string get(bool aIsRough = false)
    {
        std::string typeName;
        GetTypeNameImpl<tType>(typeName);

        if (aIsRough) 
        {
            return  typeName;
        }
        else 
        {
            std::string wName = typeName.c_str();
           /* std::string p;
            for (p = wName; (*p != '<') && (*p != 0); ++p)
                ;*/
            return  /*(*p) ? p :*/ wName;
        }
    }
};

// ***************************************************************************
//      �}�N��
//          VAR�����́A�^��^����B
//          VAR�t���́A�C���X�^���X��^����B
//          ROUGH�t���́Aconst�ƎQ�Ƃ����Ă��Ă�����ʂ��Ȃ��B
// ***************************************************************************

#define TYPENAME(dType)             TypeName<dType>::get()
#define TYPENAME_VAR(dVar)          TypeName<decltype(dVar)>::get()

#define TYPENAME_ROUGH(dType)       TypeName<dType>::get(true)
#define TYPENAME_VAR_ROUGH(dVar)    TypeName<decltype(dVar)>::get(true)