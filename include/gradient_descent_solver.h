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
    InputClass solveAcceled( InputClass x_start, double epsilon );

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
        // print
        if ( this->debug_mode_ ) {
            std::cout << i << ", ";
            std::cout << this->error_(x) << ", " ;
            for ( int i=0; i<x.rows(); i++ ) {
                std::cout << x(i) << ", ";
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
        // 探索方向 d_k を決定する
        d = calcSearchDirection( x );
        // ステップ幅を計算
        alpha = calcSearchStep( x, d );
        // 更新
        x_pre = x;
        x = x + alpha * d;
    }

    return x;
}

template <class InputClass>
inline InputClass GradientDescentSolver<InputClass>::solveAcceled( InputClass x_start, double epsilon )
{
    // 
    InputClass x = x_start;
    InputClass x_pre = x;
    double alpha = 0; // ステップ幅
    double rho = 1;
    double rho_pre = 1;
    double gamma;
    InputClass x_bar, dx;
    InputClass d; // 探索方向
    for ( long i = 0; ; i++ ) {
        // print
        if ( this->debug_mode_ ) {
            std::cout << i << ", ";
            std::cout << this->error_(x) << ", " ;
            for ( int i=0; i<x.rows(); i++ ) {
                std::cout << x(i) << ", ";
            }
            std::cout << alpha << std::endl;
        } else {
            std::cout << i << ", ";
            std::cout << this->error_(x) << ", ";
            std::cout << std::endl;
        }
        //
        if ( this->f_(x) >= this->f_(x_pre) ) {
            rho = 1;
            rho_pre = 1;
        }
        // 停止条件が満たされているかを確認
        if ( this->checkStopCondition( x, epsilon ) ) {
            break;
        }
        // x_bar
        gamma = ( rho_pre - 1 ) / rho;
        std::cout << "gamma: " << gamma << std::endl;
        dx = x - x_pre;
        x_bar = x + gamma * dx;
        //x_bar = x;
        // 探索方向 d_k を決定する
        d = calcSearchDirection( x_bar );
        // ステップ幅を計算
        alpha = calcSearchStep( x_bar, d );
        // 更新
        x_pre = x;
        x = x_bar + alpha * d;
        rho_pre = rho;
        rho = ( 1 + std::sqrt( 1 + 4 * rho_pre * rho_pre ) ) / 2.0;
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
