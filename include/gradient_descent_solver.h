// standard library
#include <iostream>
// boost
#include <boost/function.hpp>
// Eigen
#include <Eigen/Dense>

template<class InputClass>
class GradientDescentSolver
{
public:
    GradientDescentSolver(
            bool debug = true
            );
    void setf( boost::function<double(InputClass)> f );
    void setdf( boost::function<InputClass(InputClass)> df );
    void seterror( boost::function<double(InputClass)> error );
    InputClass solve( InputClass x_start, double epsilon );

protected:

    bool debug_mode_;
    boost::function<double(InputClass)> f_;
    boost::function<InputClass(InputClass)> df_;
    boost::function<double(InputClass)> error_;

    bool checkStopCondition( InputClass x, double epsilon );
    InputClass calcSearchDirection( InputClass x );
    double calcSearchStep( InputClass x, InputClass direction, double xi = 0.0001, double tau = 0.5 );
};

template <class InputClass>
inline GradientDescentSolver<InputClass>::GradientDescentSolver(
        bool debug
        )
{
    this->debug_mode_ = debug;
}

template <class InputClass>
inline void GradientDescentSolver<InputClass>::setf(
        boost::function<double(InputClass)> f
        )
{
    this->f_ = f;
}

template <class InputClass>
inline void GradientDescentSolver<InputClass>::setdf(
        boost::function<InputClass(InputClass)> df
        )
{
    this->df_ = df;
}

template <class InputClass>
inline void GradientDescentSolver<InputClass>::seterror(
        boost::function<double(InputClass)> error
        )
{
    this->error_ = error;
}

template <class InputClass>
inline InputClass GradientDescentSolver<InputClass>::solve( InputClass x_start, double epsilon )
{
    // 
    InputClass x = x_start;
    InputClass x_pre = x + 2 * epsilon * x.normalized();
    double alpha = 0; // ステップ幅
    InputClass d; // 探索方向
    for ( long i = 0; ; i++ ) {
        if ( this->debug_mode_ ) {
            std::cout << i << ", ";
            for ( int i=0; i<x.rows(); i++ ) {
                std::cout << x(i) << ", ";
            }
            std::cout << this->error_(x) << ", " << alpha << std::endl;
        }

        // 停止条件が満たされているかを確認
        if ( this->checkStopCondition( x, epsilon ) or ( x - x_pre ).norm() < epsilon ) {
            break;
        }

        // 探索方向 d_k を決定する
        d = calcSearchDirection( x );

        // ステップ幅を計算
        alpha = calcSearchStep( x, d );

        // k -> k+1
        x_pre = x;
        x += alpha * d;
    }

    return x;
}

template <class InputClass>
inline bool GradientDescentSolver<InputClass>::checkStopCondition(
        InputClass x,
        double epsilon )
{
    if ( this->error_( x ) < epsilon ) {
        return true;
    } else {
        return false;
    }
}

template <class InputClass>
inline InputClass GradientDescentSolver<InputClass>::calcSearchDirection( InputClass x )
{
    return -1 * this->df_( x );
}

template <class InputClass>
inline double GradientDescentSolver<InputClass>::calcSearchStep(
            InputClass x,
            InputClass direction,
            double xi,
            double tau
            )
{
    // armijo 条件を満たす alpha を求める
    double alpha = 1.0;
    while ( this->f_( x + alpha * direction ) > this->f_(x) + xi * alpha * this->df_( x ).dot( direction ) ) {
        alpha *= tau;
    }
    return alpha;
}
