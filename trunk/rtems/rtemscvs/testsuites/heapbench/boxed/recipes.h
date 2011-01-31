/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/* Routines from Numerical Recipes.
*/

#ifndef _RECIPES_H_
#define _RECIPES_H_

/* LU Decomposition related routines. */

/* Given a matrix a[0..n-1][0..n-1] this routine replaces it with the LU
** decomposition of a rowwise permutation of itself. a and n are input.
** a is output, arranged as in eqn 2.3.14; indx[0..n-1] is an output vector
** that records the row permutation effected by the partial pivoting; d is
** output +-1 depending on whether the number of row interchanges was even
** or odd, respectively. This routine is used in combination with LU_Back_Subst
** to solve linear systems of equations or invert a matrix. vv[0..n-1] is
** working space.
*/
extern int  	LU_Decomp(float **a, int n, int *indx, int *d, float *vv);
extern int  	LU_Decompd(double **a, int n, int *indx, int *d, double *vv);

/* Solves the set of linear eqns A.X = B. Here a[0..n-1][0..n-1] is input,
** not as the matrix A but as its LU decomposition, determined by the routine
** LU_Decomp. indx[0..n-1] is input as the permutation vector returned by
** LU_Decomp. b[0..n-1] is input as the right-hand side vector B, and returns
** with the solution X. a, n and indx are not modified by this routine
** and can be left in place for successive calls with different right-hand
** sides b. This routine takes into account the possibility that b will begin
** with many zero elements, so it is efficient for use with matrix inversion.
*/
extern void	LU_Back_Subst(float **a, int n, int *indx, float *b);
extern void	LU_Back_Substd(double **a, int n, int *indx, double *b);

/* Computes the inverse of a matrix a[0..n-1][0..n-1]. wf[0..n-1] and
** wi[0..n-1] are working space. The result is placed in out[0..n-1][0..n-1].
** The determinant of a is returned. a is destroyed by the process.
*/
extern float	LU_Invert(float **in, int n, float *wf, int *wi, float **out);
extern double	LU_Invertd(double **in,int n,double *wf,int *wi, double **out);


/* Singular value decomposition routines. */

/* Given a matrix a[0..m-1][0..n-1], this routine computes its singular value
** decomposition, A = U.W.Vtranspose. The matrix U replaces a on output. The
** diagonal matrix of singular values W is output as a vector in w[0..n-1].
** The matrix V (not the transpose) is output as v[0..n-1][0..n-1].
** temp_v[0..n-1] is temporary storage space.
*/
extern int  SV_Decomp(float **a, int m, int n, float *w, float **v,
		      float *temp_v);
extern int  SV_Decompd(double **a, int m, int n, double *w, double **v,
		       double *temp_v);

/* Solves A.X = B for a vector X, where A is specified by the arrays
** u[0..m-1][0..n-1], w[0..n-1], v[0..n-1][0..n-1] as returned by
** SVD_Decomp. m and n are the dimensions of a, and will be equal for square
** matrices. b[0..m-1] is the input right-hand side. x[0..n-1] is the output
** solution vector. No input quantities are destroyed, so the routine may be
** called sequentially with multiple b's. temp_v[0..n-1] is temporary storage.
*/
extern void SV_Back_Subst(float **u, float *w, float **v, int m, int n,
			  float *b, float *x, float *temp_v);
extern void SV_Back_Substd(double **u, double *w, double **v, int m, int n,
			   double *b, double *x, double *temp_v);


/* Linear least squares data fitting functions. */

/* Given a set of data points x[0..ndata-1][0..dim-1],y[0..ndata-1] with
** individual standard deviations sig[0..ndata-1], use chi^2 minimization to
** determine the coefficients a[0..ma-1] of the fitting function
** y = sum(a_i * afunc_i(x)). Here we solve the fitting equations using
** singular value decomposition of the ndata by ma matrix. Arrays
** u[0..ndata-1][0..ma-1], v[0..ma-1][0..ma-1] and w[0..ma-1] provide
** workspace on input. On putput they define the singular value decomposition,
** and can be used to obtain the covariance matrix. The program returns values
** for the ma fit parameters in a, and chi^2 in chisq. The user supplies
** function funcs(x, afunc) that returns the ma basis functions evaluated
** at x in the array afuncs[0..ma-1]. temp_n[0..ndata-1], temp_n1[0..ndata-1]
** and temp_ma[0..ma-1] are working space.
*/
extern void SVD_Fit(float **x, float *y, float *sig, int dim, int ndata,
		    float *a, int ma,  float **u, float **v, float *w,
		    float *chisq, void (funcs)(float*, float*),
		    float *temp_n, float *temp_n1, float *temp_ma);
extern void SVD_Fitd(double **x, double *y, double *sig, int dim, int ndata,
		     double *a, int ma,  double **u, double **v, double *w,
		     double *chisq, void (funcs)(double*, double*),
		     double *temp_n, double *temp_n1, double *temp_ma);





/* Routines for doing eigensystem calculations. */

/* Householder reduction of a real, symmetric matrix a[0..n-1][0..n-1].
** On output, a is replaced by the orthogonal matrix Q effecting the
** transformation. d[0..n-1] returns the diagonal elements of the tridiagonal
** matrix, and e[0..n-1] the off-diagonal elements, with e[0]=0.
*/
extern void Tri_Diag(float **a, int n, float *d, float *e);

/* QL algorithm woth implicit shifts, to determine the eigenvalues and
** eigenvectors of a real, symmetric, tridiagonal matrix, or of a real,
** symmetric matrix previously reduced by Tri_Diag. On input, d[0..n-1]
** contains the diagonal elements of the tridiagonal matrix. On output,
** it contains the eigenvalues. The vector e[0..n-1] inputs the sub-diagonal
** elements, with e[1] arbitrary. On output e is destroyed. The matrix
** z[0..n-1][0..n-1] should be input as the identity matrix, or the matrix
** returned by Tri_Diag if the original matrix was reduced using that function.
** On output the kth column of z contains the normalized eigenvector
** corresponding to d[k].
** Returns 1 on success, 0 on failure.
*/
extern int Tri_QL_Implicit(float *d, float *e, int n, float **z);

/*
 * Initialization routine for multi-dimensional root finding. n is the
 * number of variables (same as the number of functions).
 */
extern void Multid_Root_Init(int n);
extern void Multid_Root_Initd(int n);

/*
 * Given an n-dimensional point xold[0..n-1], the value of the functions and
 * the gradient there, fold and gl[0..n-1], and a direction pl[0..n-1], finds
 * a new point x[0..n-1] along the direction p from xoldl where the function
 * func has decreased "sufficiently". The new function value is returned
 * in f. stpmax is an input quantity that limits the length of the steps so
 * that you do not try to evaluate the function in regions where it is
 * undefined or subject to overflow. p i s usually the Newton direction.
 * The output quantity check is false (0) on normal exit. It is true (1) when
 * x is too close to xold. In a minimization algorithm, this usually signals
 * convergence and can be ignored. However, in a zero finding algorithm the
 * calling program should check whether the convergence is spurious. Some
 * difficult problems may require double precision in this routine.
 * nrfunc is the function that returns the function values at x.
 */
extern int  Line_Search(int n, float xoldl[], float fold, float gl[],
			float pl[], float x[], float *f, float stpmax,
			int *check,
			float (*func)(int, float[], float[],
				      void (*nrfunc)(int, float[], float[])),
			float fv[], void (*nrfunc)(int, float[], float[]));
extern int  Line_Searchd(int n, double xoldl[], double fold, double gl[],
			 double pl[], double x[], double *f, double stpmax,
			 int *check,
			 double (*func)(int, double[], double[],
				       void (*nrfunc)(int, double[], double[])),
			 double fv[], void (*nrfunc)(int, double[], double[]));

/*
 * Given an initial guess x[0..n-1] for a root in n dimensions, find the root
 * by a globally convergent Newton's method. The vector of functions to be
 * zerod is returned by the function vecfunc, which takes n and x as its first
 * 2 arguments and returns the functions evaluated at x in the 3rd argument.
 * The jacobian matrix is computed by the function jacobian, which takes
 * n, x and the function values at x as its first 3 arguments, and returns the
 * jacobian in its 4th argument. The output quantity check is fals (0) on a
 * normal return and true (1) if the routine has converged to a local minimum
 * of the function 0.5 * F.F. In this case try restarting from a different
 * value.
 */
extern void Multid_Newton(float x[], int n, int *check,
			  void (*vecfunc)(int, float[], float[]),
			  void (*jacobian)(int, float[], float[], float**));
extern void Multid_Newtond(double x[], int n, int *check,
			   void (*vecfunc)(int, double[], double[]),
			   void (*jacobian)(int, double[], double[], double**));



/* Given a starting point pt[0..n-1], conjugate gradient minimization is
 * performed on a function func with gradient function dfunc. The convergence
 * tolerance on the function value is input as ftol. Returned quantities
 * are pt (the location of the minimum), iter (the number of iterations
 * performed) and fret (the minimum value of the function). Working space is
 * in g[0..n-1], h[0..n-1], p[0..n-1], dp[0..n-1] and d[0..n-1].
 */
void
Conj_Grad_Min(double *pt, int n, double ftol, int *iter, double *fret,
	      double (*func)(double*), void (*dfunc)(double*, double*),
	      double *g, double *h, double *p, double *dp, double *d);



/* Numerical integration routines. */

extern void Stiff_Initialize(int n);
extern void Stiff_Set_Jacobian(void (*j)(double, double[], double[], double**,
					 int));
extern int  Stiff(double y[], double dydx[], int n, double *x, double htry,
		  double eps, double yscal[], double *hdid, double *hnext,
	          void (*derivs)(double, double[], double[]));

extern void Runge_Kutta_Initialize(int n);
extern int  Runge_Kutta(double y[], double dydx[], int n, double *x,
			double htry, double eps, double yscal[], double *hdid,
			double *hnext,
			void (*derivs)(double, double[], double[]));

extern int  ODE_Integrate(double ystart[], int nvar, double x1, double x2,
			  double eps, double h1, double hmin, int *nok,
			  int *nbad, void (*derivs)(double, double[], double[]),
              		  int (*rkqs)(double[], double[], int, double*,
				      double, double, double[], double*,
				      double*, void(*)(double, double[],
				      double[])));


/* Utility stuff. */
/* Returns sqrt(a^2 + b^2) calculated to reduce roundoff error. */
extern float	Pythag(float a, float b);
extern double	Pythagd(double a, double b);

#endif
