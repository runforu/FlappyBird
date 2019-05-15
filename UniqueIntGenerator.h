#ifndef _UNIQUEINTGENERATOR_H_
#define _UNIQUEINTGENERATOR_H_

class UniqueIntGenerator {
public:
    static int NextInt() {
        return ++m_next_int;
    }

private:
    UniqueIntGenerator() = delete;
    static int m_next_int;
};

#endif  // !_UNIQUEINTGENERATOR_H_
