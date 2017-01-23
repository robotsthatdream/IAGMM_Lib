#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <memory>
#include <vector>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#define PI 3.14159265359


namespace iagmm {

class Component{
public:

    typedef std::shared_ptr<Component> Ptr;
    typedef std::shared_ptr<const Component> ConstPtr;

    Component(){}
    Component(int dimension, double sign = 1) : _dimension(dimension), _sign(sign), _factor(0){}
    Component(const Component& c) :
        _covariance(c._covariance), _mu(c._mu), _sign(c._sign),
        _samples(c._samples), _dimension(c._dimension), _factor(c._factor){}



    void update_parameters();
    double compute_multivariate_normal_dist(Eigen::VectorXd X) const;
    Component::Ptr merge(const Component::Ptr c);
    Component::Ptr split();


    //Modifiers
    void add(Eigen::VectorXd sample){
        _samples.push_back(sample);
    }
    void clear(){_samples.clear();}

    //Statistics
    double component_score() const;
    double get_standard_deviation() const;
    std::vector<double> get_intern_estimation() const;
    void compute_eigenvalues(Eigen::VectorXd& eigenvalues, Eigen::MatrixXd& eigenvectors) const;
    double entropy();

    //Getters & Setters
    void set_factor(double f){_factor = f;}
    double get_factor() const {return _factor;}
    void set_sign(double sign){_sign = sign;}
    double get_sign() const {return _sign;}
    const Eigen::VectorXd& get_mu() const {return _mu;}
    const Eigen::VectorXd& get_sample(int i) const {return _samples[i];}
    const std::vector<Eigen::VectorXd>& get_samples() const {return _samples;}
    int size() const {return _samples.size();}
    int get_dimension() const {return _dimension;}
    const Eigen::MatrixXd& get_covariance() const {return _covariance;}

    void print_parameters() const;


    double diameter(){
        double dist , diameter = (_samples[0] - _mu).squaredNorm();
        for(auto& s : _samples){
            dist = (s - _mu).squaredNorm();
            if(dist > diameter)
                diameter = dist;
        }
        return diameter;
    }
private:
    Eigen::MatrixXd _covariance;
    Eigen::VectorXd _mu;
    std::vector<Eigen::VectorXd> _samples;
    int _dimension;
    double _sign;
    double _factor;
};

}

#endif //COMPONENT_HPP