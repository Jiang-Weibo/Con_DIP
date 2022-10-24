#ifndef MYCOMPLEX_H
#define MYCOMPLEX_H
template<typename T>
class MyComplex{
public:
    T rel;
    T img;
    MyComplex() {
        rel = 0.0;
        img = 0.0;
    }
    MyComplex(T arg1, T arg2) {
        rel = arg1;
        img = arg2;
    }
    MyComplex operator+(MyComplex a){
        MyComplex ans;
        ans.rel = this->rel + a.rel;
        ans.img = this->img + a.img;
        return ans;
    }
    MyComplex operator-(MyComplex a) {
        MyComplex ans;
        ans.rel = this->rel - a.rel;
        ans.img = this->img - a.img;
        return ans;
    }
    MyComplex operator*(MyComplex a) {
        MyComplex ans;
        ans.rel = this->rel * a.rel - this->img * a.img;
        ans.img = this->rel * a.img + this->img * a.rel;
        return ans;
    }
};
#endif // MYCOMPLEX_H
