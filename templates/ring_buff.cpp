template<typename T, size_t n>
class ring_buffer {
    T buff[n];
    size_t f = n-1;
    size_t b = 0;

public:

    inline const T &front() const { return buff[f]; }

    inline const T &back() const { return buff[b]; }

    inline void push_front(const T &v) { buff[f = f == n-1 ? 0 : f+1] = v; }
    inline void push_front(T &&v) { buff[f = f == n-1 ? 0 : f+1] = v; }

    inline void push_back(const T &v) { buff[b = b == 0 ? n-1 : b-1] = v; }
    inline void push_back(T &&v) { buff[b = b == 0 ? n-1 : b-1] = v; }

    inline void pop_front() { f = f == 0 ? n-1 : f-1; }

    inline void pop_back() { b = b == n-1 ? 0 : b+1; }

    inline bool empty() const { return (b ? b-1 : n-1) == f; }

    string dump() {
        if (empty()) return "";
        ostringstream s;
        for (size_t i = b; i != f; i = i == n-1 ? 0 : i+1) {
            s << buff[i] << " ";
        }
        s << buff[f];
        return s.str();
    }
};