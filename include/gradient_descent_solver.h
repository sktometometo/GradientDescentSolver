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
            bool debug = true
            );
    void setf( boost::function<double(InputClass)> f );
    void setdf( boost::function<InputClass(InputClass)> df );
    void seterror( boost::function<double(InputClass)> error );
    InputClass solve( InputClass x_start, std::vector<InputClass>& trajectory, double epsilon, int accel=0 );

protected:

    bool debug_mode_;
    boost::function<double(InputClass)> f_;
    boost::function<InputClass(InputClass)> df_;
    boost::function<double(InputClass)> error_;

    bool checkStopCondition( InputClass x, double epsilon );
    InputClass calcSearchDirection( InputClass x );
    double calcSearchStep( InputClass x, InputClass direction, double xi = 0.01, double tau = 0.5 );
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
inline InputClass GradientDescentSolver<InputClass>::solve(
        InputClass x_start,
        std::vector<InputClass>& trajectory,
        double epsilon,
        int accel
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
                d = calcSearchDirection( x );
                alpha = calcSearchStep( x, d );
                break;
            case 2:
                x_bar = x + ( ( rho_pre - 1 ) / rho ) * ( x - x_pre );
                d = calcSearchDirection( x_bar );
                alpha = calcSearchStep( x_bar, d );
                break;
            default:
                d = calcSearchDirection( x );
                alpha = calcSearchStep( x, d );
                break;
        }

        // print
        if ( this->debug_mode_ ) {
            std::cout << i << ", ";
            std::cout << this->error_(x) << ", " ;
            for ( int i=0; i<x.rows(); i++ ) {
                std::cout << x(i) << ", ";
            }
            for ( int i=0; i<d.rows(); i++ ) {
                std::cout << d(i) << ", ";
            }
            std::cout << alpha << std::endl;
        } else {
            std::cout << i << ", ";
            std::cout << this->error_(x) << ", ";
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
    while ( not this->f_( x + alpha * direction ) - this->f_(x) <= xi * alpha * this->df_( x ).dot( direction ) ) {
        alpha *= tau;
    }
    return alpha;
}
