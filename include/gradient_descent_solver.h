// standard library
#include <iostream>
#include <set>
// boost
#include <boost/function.hpp>
// Eigen
#include <Eigen/Dense>

template<class InputClass>
class GradientDescentSolver
{
public:
    GradientDescentSolver(
            int debug = 0
            );
    void setf( boost::function<double(InputClass)> f );
    void setdf( boost::function<InputClass(InputClass)> df );
    void seterror( boost::function<double(InputClass)> error );
    InputClass solve( InputClass x_start,
                      std::vector<InputClass>& trajectory,
                      double epsilon,
                      int accel=0,
                      double xi = 0.5, 
                      double tau = 0.5,
                      double learning_rate = 1.0 );

protected:

    int debug_mode_; // 0: not print, 1: print briefly, 2: print detaily
    boost::function<double(InputClass)> f_;
    boost::function<InputClass(InputClass)> df_;
    boost::function<double(InputClass)> error_;

    bool checkStopCondition( InputClass x, double epsilon );
    InputClass calcSearchDirection( InputClass x, double learning_rate );
    double calcSearchStep( InputClass x, InputClass direction, double xi, double tau );
};

template <class InputClass>
inline GradientDescentSolver<InputClass>::GradientDescentSolver(
        int debug
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
inline InputClass GradientDescentSolver<InputClass>::solve(
        InputClass x_start,
        std::vector<InputClass>& trajectory,
        double epsilon,
        int accel,
        double xi,
        double tau,
        double learning_rate
        )
{
    // 
    InputClass x = x_start;
    InputClass x_pre = x_start;
    InputClass x_bar, dx;
    double alpha = 0; // ステップ幅
    double rho = 1;
    double rho_pre = 1;
    InputClass d = x - x; // 探索方向

    //
    for ( long i = 0; ; i++ ) {
        //
        trajectory.push_back( x );

        // 探索方向 d_k を決定し, ステップ幅を決定する
        switch ( accel ) {
            case 1:
                d = calcSearchDirection( x, learning_rate );
                alpha = calcSearchStep( x, d, xi, tau );
                break;
            case 2:
                dx = x - x_pre;
                x_bar = x + ( ( rho_pre - 1 ) / rho ) * dx;
                d = calcSearchDirection( x_bar, learning_rate );
                alpha = calcSearchStep( x_bar, d, xi, tau );
                break;
            default:
                d = calcSearchDirection( x, learning_rate );
                alpha = calcSearchStep( x, d, xi, tau );
                break;
        }

        // print
        switch ( this->debug_mode_ ) {
            case 1:
                std::cout << i << ", ";
                std::cout << this->error_(x) << ", ";
                std::cout << std::endl;
                break;
            case 2:
                std::cout << i << ", ";
                std::cout << this->error_(x) << ", " ;
                /*
                if ( accel == 2 ) {
                    std::cout << rho << ", " ;
                    std::cout << rho_pre << ", " ;
                    std::cout << ( ( rho_pre - 1 ) / rho ) << ", ";
                }
                */
                std::cout << alpha << ", ";
                for ( int i=0; i<x.rows(); i++ ) {
                    std::cout << x(i) << ", ";
                }
                for ( int i=0; i<d.rows(); i++ ) {
                    std::cout << d(i) << ", ";
                }
                std::cout << std::endl;
        }

        // 停止条件が満たされているかを確認
        if ( this->checkStopCondition( x, epsilon ) ) {
            break;
        }

        // 更新
        switch( accel ) {
            case 1:
                dx = x - x_pre;
                x_pre = x;
                x = x + alpha * d + 0.9 * dx;
                break;
            case 2:
                rho_pre = rho;
                rho = ( 1 + std::sqrt( 1 + 4 * rho * rho ) ) / 2;
                x_pre = x;
                x = x_bar + alpha * d;
                break;
            default:
                x_pre = x;
                x = x + alpha * d;
                break;
        }
    }

    return x;
}

template <class InputClass>
inline bool GradientDescentSolver<InputClass>::checkStopCondition(
        InputClass x,
        double epsilon )
{
    if ( this->error_( x ) < epsilon or isnan( x.norm() ) ) {
        return true;
    } else {
        return false;
    }
}

template <class InputClass>
inline InputClass GradientDescentSolver<InputClass>::calcSearchDirection( InputClass x, double learning_rate )
{
    return -1 * learning_rate * this->df_( x );
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
    // この条件をいじると収束しなくなる?
    // 同値な条件のはずなのに....
    while ( not ( this->f_( x + alpha * direction ) - this->f_(x) <= xi * this->df_( x ).dot( alpha * direction ) ) ) {
        alpha *= tau;
    }
    return alpha;
}
