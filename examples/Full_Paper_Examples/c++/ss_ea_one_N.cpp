#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
//#include "directfn_quad.h"
#include "directfn_defs.h"
#include "directfn_contour.h"
#include "directfn_algorithm_ea.h"
#include "directfn_kernel_quad_vect.h"

using  std::cout;
using  std::endl;
using  std::setprecision;

using Directfn::Quadrilateral_EA;
using Directfn::SingularContour3xn;
using Directfn::dcomplex;
using Directfn::QuadrilateralKernel_PlanarVectorSS;
using Directfn::relative_error;
using Directfn::max_element;

#ifndef M_PI
using Directfn::M_PI;
#endif

#ifndef DBL_EPSILON
using Directfn::DBL_EPSILON;
#endif

///////////////////////////////////////////////////////////////////////////////
void  ss_ea_one_N() noexcept {

	double T;

	const double k0wn = 2 * M_PI;

	const double d = 0.1;
	const size_t  N_ref = 20;
	const int len = 16;

	double r1[] = { 0.0 , 0.0 , 0.0 };
	double r2[] = { d , 0.0 , 0.0 };
	double r3[] = { d,  d , 0.0 };
	double r4[] = { 0.0 , d , 0.0 };
	double r5[] = { d , 0.0 , d };
	double r6[] = { d , d , d };

	double max_Error_0, max_Error_1, max_Error_2, max_Error_3, max_Error_4;
	double * Error = new double[len];
	const dcomplex * I_quad;
	dcomplex * I_ref_value = new dcomplex[len];



	SingularContour3xn Q;

    Q.set_points(r1, r4, r3, r2, r5, r6);
//    Q.set_points(r4, r1, r2, r3, r6, r5);

	unique_ptr<Quadrilateral_EA<QuadrilateralKernel_PlanarVectorSS>> up_quad_ea(new Quadrilateral_EA<QuadrilateralKernel_PlanarVectorSS>());

	// Setting parameters
	up_quad_ea->set_wavenumber(k0wn);
	up_quad_ea->set(Q);
	up_quad_ea->set_Gaussian_orders_4(N_ref, N_ref, N_ref, N_ref);

	// Calculating reference value
	cout << "Computing reference values ..." << endl;
	std::chrono::steady_clock::time_point  begin_t_quad = std::chrono::steady_clock::now();
	up_quad_ea->calc_Iss();
	std::chrono::steady_clock::time_point  end_t_quad = std::chrono::steady_clock::now();

	T = std::chrono::duration_cast<std::chrono::milliseconds>(end_t_quad - begin_t_quad).count();
	cout << "Reference value computed in " << T << " milliseconds" << endl;

	up_quad_ea->copy_Iss_array_values_to(I_ref_value);

	/* // Output reference value
	cout << "I_ref = " << endl;
    for (size_t i = 0; i < up_quad_ea->kernel_size(); ++i) {
        cout << setprecision(20) << up_quad_ea->Iss_arr(i) << endl;
    }
	*/

	std::ofstream myfile;
	myfile.open("Results_ss_ea_one_N.txt");

	const int Counter = 20;

	cout << "Convergence test starting ..." << endl;
	for (int N = 1; N <= Counter; N++)
	{

		cout << "N: " << N << endl;
		cout << endl;

		////////////////////////////Same N //////////////////////////////////

		// Setting Gaussian orders
		up_quad_ea->set_Gaussian_orders_4(N, N, N, N);
		up_quad_ea->calc_Iss();
		I_quad = up_quad_ea->Iss();

     
		for (int i = 0; i < len; i++)
		{
			Error[i] = relative_error(I_quad[i], I_ref_value[i]);
		}

		max_Error_0 = max_element(Error, len);

		////////////////////////////N 20 20 20 //////////////////////////////////
		// Setting Gaussian orders
		up_quad_ea->set_Gaussian_orders_4(N, 20, 20, 20);
		up_quad_ea->calc_Iss();
		I_quad = up_quad_ea->Iss();




		for (int i = 0; i < len; i++)
		{
			Error[i] = relative_error(I_quad[i], I_ref_value[i]);
		}

		max_Error_1 = max_element(Error, len);

		////////////////////////////20 N 20 20 //////////////////////////////////

		// Setting Gaussian orders
		up_quad_ea->set_Gaussian_orders_4(20, N, 20, 20);
		up_quad_ea->calc_Iss();
		I_quad = up_quad_ea->Iss();




		for (int i = 0; i < len; i++)
		{
			Error[i] = relative_error(I_quad[i], I_ref_value[i]);
		}

		max_Error_2 = max_element(Error, len);

		////////////////////////////20 20 N 20 //////////////////////////////////

		// Setting Gaussian orders
		up_quad_ea->set_Gaussian_orders_4(20, 20, N, 20);
		up_quad_ea->calc_Iss();
		I_quad = up_quad_ea->Iss();




		for (int i = 0; i < len; i++)
		{
			Error[i] = relative_error(I_quad[i], I_ref_value[i]);
		}

		max_Error_3 = max_element(Error, len);

		////////////////////////////20 20 20 N //////////////////////////////////

		// Setting Gaussian orders
		up_quad_ea->set_Gaussian_orders_4(20, 20, 20, N);
		up_quad_ea->calc_Iss();
		I_quad = up_quad_ea->Iss();




		for (int i = 0; i < len; i++)
		{
			Error[i] = relative_error(I_quad[i], I_ref_value[i]);
		}

		max_Error_4 = max_element(Error, len);



		// Writing results to file
		myfile << N << " " << setprecision(20) << max_Error_0 << " " << setprecision(20) << max_Error_1 << " " << setprecision(20) << max_Error_2 << " ";
		myfile << setprecision(20) << max_Error_3 << " " << setprecision(20) << max_Error_4 << endl;
	}

	myfile.close();
	cout << "Convergence test completed." << endl;


}

int main(int, char *[]) {

	ss_ea_one_N();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// End of the file





