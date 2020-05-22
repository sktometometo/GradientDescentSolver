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
            boost::function<double(InputClass)> f,
            bool debug = true
            );
    void set( boost::function<double(InputClass)> f );
    InputClass solve( InputClass x_start, double epsilon );

protected:

    bool debug_mode_;
    boost::function<double(InputClass)> f_;

    InputClass df( InputClass x, double dx );
    bool checkStopCondition( InputClass x, InputClass x_pre, double epsilon );
    InputClass calcSearchDirection( InputClass x );
    double calcSearchStep( InputClass x, InputClass direction, double xi = 0.3, double tau = 0.9 );
};

template <class InputClass>
inline GradientDescentSolver<InputClass>::GradientDescentSolver(
        boost::function<double(InputClass)> f,
        bool debug
        )
{
    this->set( f );
    this->debug_mode_ = debug;
}

template <class InputClass>
inline void GradientDescentSolver<InputClass>::set(
        boost::function<double(InputClass)> f
        )
{
    this->f_ = f;
}

template <class InputClass>
inline InputClass GradientDescentSolver<InputClass>::solve( InputClass x_start, double epsilon )
{
    // 
    InputClass x = x_start;
    InputClass x_pre = x_start + 2 * epsilon * x_start.normalized();
    double alpha = 0; // ステップ幅
    InputClass d; // 探索方向
    for ( long i = 0; ; i++ ) {
        if ( this->debug_mode_ ) {
            std::cout << i << ", ";
            for ( int i=0; i<x.rows(); i++ ) {
                std::cout << x(i) << ", ";
            }
            std::cout << std::endl;
        }

        // 停止条件が満たされているかを確認
        if ( this->checkStopCondition( x, x_pre, epsilon ) ) {
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
inline InputClass GradientDescentSolver<InputClass>::df( InputClass x, double dx )
{
    InputClass result = x;
    for ( int i; i < x.rows(); i++ ) {
        InputClass x_dx = x;
        x_dx(i) -= dx;
        result(i) = ( this->f_(x) - this->f_(x_dx) ) / dx;
    }
    return result;
}

template <class InputClass>
inline bool GradientDescentSolver<InputClass>::checkStopCondition(
        InputClass x,
        InputClass x_pre,
        double epsilon )
{
    double dx = x.norm() / 100;
    if ( std::abs( this->df( x, dx ).norm() ) < epsilon or ( x - x_pre ).norm() < epsilon ) {
        return true;
    } else {
        return false;
    }
}

template <class InputClass>
inline InputClass GradientDescentSolver<InputClass>::calcSearchDirection( InputClass x )
{
    double dx = x.norm() / 100;
    return -1 * this->df( x, dx ).normalized();
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
    double dx = x.norm() / 100;
    while ( this->f_( x + alpha * direction ) > this->f_(x) + xi * alpha * this->df( x, dx ).dot( direction ) ) {
        alpha *= tau;
    }
    return alpha;
}
