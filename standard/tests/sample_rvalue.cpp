#include <cstdio>
#include <cstdlib>
#include <new>
#include <utility>

// C++のムーブと完全転送を知る
// https://proc-cpuinfo.fixstars.com/2016/03/c-html/
// 本当は怖くないムーブセマンティクス
// https://yohhoy.hatenablog.jp/entry/2012/12/15/120839

namespace
{
class Mem
{
  private:
    char val_;

  public:
    Mem()
    {
        val_ = 'a';
    }

  public:
    void *operator new(std::size_t n)
    {
        void *p = std::malloc(n);
        printf("  new %p\n", p);
        return p ? p : throw std::bad_alloc();
    }
    void *operator new[](std::size_t n)
    {
        void *p = std::malloc(n);
        printf("  new[] %p\n", p);
        return p ? p : throw std::bad_alloc();
    }
    void operator delete(void *p) noexcept
    {
        printf("  delete %p\n", p);
        std::free(p);
    }
    void operator delete[](void *p) noexcept
    {
        printf("  delete[] %p\n", p);
        std::free(p);
    }
};

class Hoge
{
  private:
    int m_value_ = 0;
    Mem *m_mem_ = nullptr;

  public:
    // コンストラクタ
    Hoge()
    {
        printf("  Hoge::Constructor\n");
        m_value_ = 100;
        m_mem_ = new Mem[1000];
    }
    // デストラクタ
    ~Hoge()
    {
        printf("  Hoge::Destructor\n");
        if (m_mem_ != nullptr)
        {
            delete[] m_mem_;
        }
    }
    // コピーコンストラクタ
    Hoge(const Hoge &org)
    {
        printf("  Hoge::Copy Constructor\n");
        *this = org;
    }
    // コピー代入演算子
    Hoge &operator=(const Hoge &org)
    {
        printf("  Hoge::Copy Operator\n");
        if (this != &org)
        {
            m_value_ = org.m_value_;
            if (m_mem_ != nullptr)
            {
                delete[] m_mem_;
            }
            if (org.m_mem_ != nullptr)
            {
                m_mem_ = new Mem[1000];
            }
        }
        return *this;
    }
    // ムーブコンストラクタ
    Hoge(Hoge &&org)
    {
        printf("  Hoge::Move Constructor\n");
        *this = std::move(org);
    }
    // ムーブ代入演算子
    Hoge &operator=(Hoge &&org)
    {
        printf("  Hoge::Move Operator\n");
        if (this != &org)
        {
            if (m_mem_ != nullptr)
            {
                delete[] m_mem_;
            }
            m_value_ = org.m_value_;
            m_mem_ = org.m_mem_;
            org.m_mem_ = nullptr;
        }
        return *this;
    }

    // valueの設定（左辺値参照）
    void set(const int &value)
    {
        printf("  Hoge::set lvalue\n");
        m_value_ = value;
    }
    // valueの設定（右辺値参照）
    void set(int &&value)
    {
        printf("  Hoge::set rvalue\n");
        m_value_ = std::move(value);
    }

    void devnull()
    {
        //何もしない
    }
};

void f(Hoge h)
{
    h.devnull();
}

// F(T v)だとコピーされるので完全転送にならない
// F(T &v)だと右辺値を渡せずにビルドエラーになる
template <typename T>
void F(T &&v)
{
    // f(v); // 右辺値が取れないので全てコピーされる
    // f(std::move(v)); // 左辺値が取れないので全てムーブされる
    f(std::forward<T>(v)); // OK

    // 参照元が右辺値なら、引数を右辺値にキャストする
    // 参照元が左辺値なら、引数を左辺値にキャストする
}

} // namespace

void sample_rvalue()
{
    printf("<< sample_rvalue >>\n");
    {
        printf("* Copy\n");
        // h1のコンストラクタが呼ばれ、m_mem_がnewされる
        Hoge h1;
        // h2のコンストラクタが呼ばれ、m_mem_がnewされる
        Hoge h2;
        // h2のコピー代入演算子が呼ばれる
        // h2のm_mem_がdelete,newされて、h1のm_mem_がハードコピーされる
        h2 = h1;
        // h3のコピーコンストラクタが呼ばれる
        // h3のm_mem_がnewされて、h1のm_mem_がハードコピーされる
        Hoge h3 = h1;
        // h3のデストラクタが呼ばれ、m_mem_がdeleteされる
        // h2のデストラクタが呼ばれ、m_mem_がdeleteされる
        // h1のデストラクタが呼ばれ、m_mem_がdeleteされる
    }
    {
        printf("* Move\n");
        // h1のコンストラクタが呼ばれ、m_mem_がnewされる
        Hoge h1;
        // h2のコンストラクタが呼ばれ、m_mem_がnewされる
        Hoge h2;
        // h2のムーブ代入演算子が呼ばれる
        // h2のm_mem_がdeleteされて、h1のm_mem_がソフトコピーされる
        h2 = std::move(h1);
        // h3のムーブコンストラクタが呼ばれる
        // h3のm_mem_に、h2のm_mem_がソフトコピーされる
        Hoge h3 = std::move(h2);
        // h3のデストラクタが呼ばれ、m_mem_がdeleteされる
        // h2のデストラクタが呼ばれる（m_mem_はnullptrなのでdeleteされない）
        // h1のデストラクタが呼ばれる（m_mem_はnullptrなのでdeleteされない）
    }
    {
        printf("* lvalue rvalue\n");
        int value = 10;
        const int cvalue = 10;
        Hoge h1;
        h1.set(value);             // 左辺値
        h1.set(10);                // 右辺値
        h1.set(value + 1);         // 右辺値
        h1.set(std::move(value));  // 右辺値(moveで右辺値にキャスト)
        h1.set(std::move(cvalue)); // 左辺値(const値は右辺値にキャストできない)
    }
    {
        printf("* forward\n");
        Hoge h;
        F(h);            //左辺値なのでstd::forwardによって左辺値キャストされる
        F(Hoge());       //右辺値なのでstd::forwardによって右辺値キャストされる
        F(std::move(h)); //右辺値なのでstd::forwardによって右辺値キャストされる
    }
}
