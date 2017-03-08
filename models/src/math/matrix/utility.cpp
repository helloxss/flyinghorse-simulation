#include <cmath>

#include <armadillo>

#include "aux/global_constants.hh"

#include "math/matrix/utility.hh"

arma::vec3 pol_from_cart(arma::vec3 in)
{
    double d = 0.0;
    double azimuth = 0.0;
    double elevation = 0.0;
    double denom;
    arma::vec3 POLAR;

    double v1 = in(0);
    double v2 = in(1);
    double v3 = in(2);

    d = sqrt(v1 * v1 + v2 * v2 + v3 * v3);
    azimuth = atan2(v2, v1);

    denom = sqrt(v1 * v1 + v2 * v2);
    if (denom > 0.)
        elevation = atan2(-v3, denom);
    else {
        if (v3 > 0)
            elevation = -PI / 2.;
        if (v3 < 0)
            elevation = PI / 2.;
        if (v3 == 0)
            elevation = 0.;
    }

    POLAR(0) = d;
    POLAR(1) = azimuth;
    POLAR(2) = elevation;

    return POLAR;
}

///////////////////////////////////////////////////////////////////////////////
// Returns the angle between two 3x1 vectors
// Example: theta=angle(VEC1,VEC2);
// 010824 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
double angle(arma::vec3 VEC1, arma::vec3 VEC2)
{
    double argument;
    double scalar = dot(VEC1, VEC2);
    double abs1 = norm(VEC1);
    double abs2 = norm(VEC2);

    double dum = abs1 * abs2;
    if (abs1 * abs2 > arma::datum::eps)
        argument = scalar / dum;
    else
        argument = 1.;
    if (argument > 1.)
        argument = 1.;
    if (argument < -1.)
        argument = -1.;

    return acos(argument);
}

///////////////////////////////////////////////////////////////////////////////
// Return skew symmetric matrix of a Vector3
//            | 0 -c  b|        |a|
//            | c  0 -a| <--    |b|
//            |-b  a  0|        |c|
//
// Example: MAT = VEC.skew_sym();
///////////////////////////////////////////////////////////////////////////////
arma::mat33 skew_sym(arma::vec3 vec)
{
    arma::mat33 RESULT;

    RESULT(0, 0) = 0;
    RESULT(1, 0) = vec(2);
    RESULT(2, 0) = -vec(1);
    RESULT(0, 1) = -vec(2);
    RESULT(1, 1) = 0;
    RESULT(2, 1) = vec(0);
    RESULT(0, 2) = vec(1);
    RESULT(1, 2) = -vec(0);
    RESULT(2, 2) = 0;

    return RESULT;
}

////////////////////////////////////////////////////////////////////////////////
// Returns the T.M. of the psivg -> thtvg sequence
//
// 010628 Created by Peter H Zipfel
// 170121 Create Armadillo Version by soncyang
////////////////////////////////////////////////////////////////////////////////
arma::mat33 build_psivg_thtvg_TM(const double &psivg, const double &thtvg)
{
    arma::mat33 AMAT;

    AMAT(0, 2) = -sin(thtvg);
    AMAT(1, 0) = -sin(psivg);
    AMAT(1, 1) = cos(psivg);
    AMAT(2, 2) = cos(thtvg);
    AMAT(0, 0) = (AMAT(2, 2) * AMAT(1, 1));
    AMAT(0, 1) = (-AMAT(2, 2) * AMAT(1, 0));
    AMAT(2, 0) = (-AMAT(0, 2) * AMAT(1, 1));
    AMAT(2, 1) = (AMAT(0, 2) * AMAT(1, 0));
    AMAT(1, 2) = 0.0;

    return AMAT;
}

////////////////////////////////////////////////////////////////////////////////
// Returns the T.M. of the psi->tht->phi sequence
// Euler angle transformation matrix of flight mechanics
//
// 011126 Created by Peter H Zipfel
// 170121 Add Armadillo version
////////////////////////////////////////////////////////////////////////////////
arma::mat33 build_psi_tht_phi_TM(const double &psi, const double &tht, const double &phi)
{
    double spsi = sin(psi);
    double cpsi = cos(psi);
    double stht = sin(tht);
    double ctht = cos(tht);
    double sphi = sin(phi);
    double cphi = cos(phi);

    arma::mat33 AMAT;
    AMAT(0, 0) = cpsi * ctht;
    AMAT(1, 0) = cpsi * stht * sphi - spsi * cphi;
    AMAT(2, 0) = cpsi * stht * cphi + spsi * sphi;
    AMAT(0, 1) = spsi * ctht;
    AMAT(1, 1) = spsi * stht * sphi + cpsi * cphi;
    AMAT(2, 1) = spsi * stht * cphi - cpsi * sphi;
    AMAT(0, 2) = -stht;
    AMAT(1, 2) = ctht * sphi;
    AMAT(2, 2) = ctht * cphi;

    return AMAT;
}

arma::vec4 Matrix2Quaternion(arma::mat33 Matrix_in)
{
    double q_square[4];
    double q_square_max;
    int i, j;
    arma::vec4 Quaternion;
    Matrix_in = trans(Matrix_in);
    q_square[0] = fabs(1.0 + Matrix_in(0,0) + Matrix_in(1,1) + Matrix_in(2,2));
    q_square[1] = fabs(1.0 + Matrix_in(0,0) - Matrix_in(1,1) - Matrix_in(2,2));
    q_square[2] = fabs(1.0 - Matrix_in(0,0) + Matrix_in(1,1) - Matrix_in(2,2));
    q_square[3] = fabs(1.0 - Matrix_in(0,0) - Matrix_in(1,1) + Matrix_in(2,2));

    q_square_max = q_square[0];
    j = 0;
    for(i = 1;i < 4;i++)
    {
        if(q_square[i] > q_square_max)
        {
            q_square_max = q_square[i];
            j = i;
        }
    }
    switch (j)
    {
        case 0:
                Quaternion(0) = 0.5 * sqrt(q_square_max);
                Quaternion(1) = 0.25 * (Matrix_in(2,1) - Matrix_in(1,2)) / Quaternion[0];
                Quaternion(2) = 0.25 * (Matrix_in(0,2) - Matrix_in(2,0)) / Quaternion[0];
                Quaternion(3) = 0.25 * (Matrix_in(1,0) - Matrix_in(0,1)) / Quaternion[0];
                break;
        case 1:
                Quaternion(1) = 0.5 * sqrt(q_square_max);
                Quaternion(0) = 0.25 * (Matrix_in(2,1) - Matrix_in(1,2)) / Quaternion[1];
                Quaternion(2) = 0.25 * (Matrix_in(1,0) + Matrix_in(0,1)) / Quaternion[1];
                Quaternion(3) = 0.25 * (Matrix_in(0,2) + Matrix_in(2,0)) / Quaternion[1];
                break;
        case 2:
                Quaternion(2) = 0.5 * sqrt(q_square_max);
                Quaternion(0) = 0.25 * (Matrix_in(0,2) - Matrix_in(2,0)) / Quaternion[2];
                Quaternion(1) = 0.25 * (Matrix_in(1,0) + Matrix_in(0,1)) / Quaternion[2];
                Quaternion(3) = 0.25 * (Matrix_in(2,1) + Matrix_in(1,2)) / Quaternion[2];
                break;
        case 3:
                Quaternion(3) = 0.5 * sqrt(q_square_max);
                Quaternion(0) = 0.25 * (Matrix_in(1,0) - Matrix_in(0,1)) / Quaternion[3];
                Quaternion(1) = 0.25 * (Matrix_in(2,0) + Matrix_in(0,2)) / Quaternion[3];
                Quaternion(2) = 0.25 * (Matrix_in(2,1) + Matrix_in(1,2)) / Quaternion[3];
                break;


    }

    return Quaternion;
}

arma::mat33 Quaternion2Matrix(arma::vec4 Quaternion_in)
{
    arma::mat33 Matrix_out;

    Matrix_out(0,0) = 2. * (Quaternion_in(0) * Quaternion_in(0) + Quaternion_in(1) * Quaternion_in(1)) - 1.;
    Matrix_out(0,1) = 2. * (Quaternion_in(1) * Quaternion_in(2) + Quaternion_in(0) * Quaternion_in(3));
    Matrix_out(0,2) = 2. * (Quaternion_in(1) * Quaternion_in(3) - Quaternion_in(0) * Quaternion_in(2));
    Matrix_out(1,0) = 2. * (Quaternion_in(1) * Quaternion_in(2) - Quaternion_in(0) * Quaternion_in(3));
    Matrix_out(1,1) = 2. * (Quaternion_in(0) * Quaternion_in(0) + Quaternion_in(2) * Quaternion_in(2)) - 1.;
    Matrix_out(1,2) = 2. * (Quaternion_in(2) * Quaternion_in(3) + Quaternion_in(0) * Quaternion_in(1));
    Matrix_out(2,0) = 2. * (Quaternion_in(1) * Quaternion_in(3) + Quaternion_in(0) * Quaternion_in(2));
    Matrix_out(2,1) = 2. * (Quaternion_in(2) * Quaternion_in(3) - Quaternion_in(0) * Quaternion_in(1));
    Matrix_out(2,2) = 2. * (Quaternion_in(0) * Quaternion_in(0) + Quaternion_in(3) * Quaternion_in(3)) - 1.;

    return Matrix_out;
}
