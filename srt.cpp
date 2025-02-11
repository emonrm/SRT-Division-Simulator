
/*
* Author: Mahbubur Rahman
* r.mahbub@wayne.edu
*/



#include <iostream>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

#define debug false		// only shows the results after applying SRT method
#define simulation true	// enable to see all the simulaiton steps inside SRT method
#define dout debug && cout
#define sout simulation && cout

int total_cost = 0; // counts the total execution time in delta_t

/////////////////////// Compound result structure ///////////////////
//self explanatory
class result{
	public:
	string remainder, quotient;
	int execution_time;
};
/////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// print helper functions////////////////////////////////////////////////////////////////////////////////////////////////
void println(){dout << endl;}
//specilized print function: decreses visual representation complexity for showing args in console
void println (string str){if(str[0] == '1'){str.insert(1, ".");}else if (str[0] == '0') {str[0] = '.';}else{}dout << str << endl;}
//specilized print function: decreses visual representation complexity for showing args in console
void print(string str){if(str[0] == '1'){str.insert(1, ".");}else if (str[0] == '0'){str[0] = '.';}else{}dout << " " << str << " ";}
//prints in special format to show simulaiton steps
void print_simulation_step(string description, string str, int b = 0, bool ir = false, bool st = false){
	int w = 5; int x = 15;
	if((str[0] == '1') && !st){str.insert(1, "."); w--;}else if (str[0] == '0') {str[0] = '.';}else{}
	if (!b) str.insert(str.length()/2 + 1, "  ");
	if(b == 1) {str = "\33[4m" + str + "\33[0m";}
	sout << std::setw(25) << std::left << description;
	sout << std::setw(w) << std::left << ":";
	sout << str << std::setw(x) << std::right;
	if ((!b && !ir) || (b == 2 && ir)) sout << total_cost << " Δt";
	sout << endl;
}
//print any label in middle of the window
void print_simulation_step(string step, int w){sout << std::setw(w) << std::right << "\33[4m" <<step << "\33[0m" << endl;}
//just a new line in simulaiton step
void print_simulation_step(){sout << endl;}
//print the result in proper format
void println(string aq, string b, result res){
	dout<<"A = "<<aq<<"\tB = "<<b<<"\nRemainder = "<<res.remainder<<"\tQuotient = "<<res.quotient<<"\tT = "<<res.execution_time<<" Δt.";
	dout << endl << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////// check divide overflow function ////////////////////////
// arg1 is value of A, arg2 is the value of B
// returns 1 if devide by zero overflow occurs
// returns 2 if a >= b overflow occurs,
// 0 is no overflow 
int arg_check_overflow(const string & arg1, const string & arg2){
	int overflow = 1; //assuming devide by zero overflow will occur

	// check if B is zero
	for (int i = 1; i < arg2.length(); i++){
		if(arg2[i] == '1'){
			overflow = 0; // B has at least one '1' bit
			break;
		}
	}
	if (overflow){ return overflow;	}							// return divide by zero overflow if occurs;

	//check if A >= B
	//Assuming this overflow will occur
	overflow = 2;	
	for(int j = 1; j < arg2.length(); j++){						// if no condition is satisfied inside forloop, A = B
		if((arg1[j] != arg2[j]) && (arg2[j] == '1')){			// confirms that B > A
			overflow = 0;
			break;
		}
		else if((arg1[j] != arg2[j]) && (arg2[j] == '0')){		// confirms that A > B
			break;
		}
	}
	return overflow;
}
///////////////////////////////////////////////////////////////////////////////


////////////////////// Normalize Args/////////////////////////
// arg: inplace normalization pointer for argmuent
// factor: number of bits normalized
// mode: defines whether normalizing or adjusting. default: normalization
void arg_normalize(string & arg, int & factor, int mode = 0){
	// normalize upto normalization factor
	if (mode == 1){
		int j = 1;
		while (j <= factor)
		{
			for(int i = 1; i < arg.length()-1; i++){
				arg[i] = arg[i+1];
			}
			arg[arg.length()-1] = '*';
			j++;
			total_cost += 3;							// each shift for adjustment costs 3dt
		}
	}
	// normalize and find the normalization factor
	else {
		while (arg[1] == '0'){
			for (int i = 1; i < arg.length()-1; i ++){
				arg[i] = arg[i+1];
			}
			arg[arg.length()-1] = '0';
			factor++;
			total_cost += 3; 							// each shift takes 3 dt time
		}
	}
}
//////////////////////////////////////////////////////////////

////////////////////// Compliment function ///////////////////
// arg1: unsigned value
// arg2: 2's compliment
void arg_compliment(const string & arg1, string & arg2){
	//string arg2 = arg1;
	total_cost += (arg1.length() -1);				// cost for 2's complitent , n * dt , n : numer of bits in arg1
	arg2 = arg1;
	arg2[0] = '1'; 	// represents negetive sign
	for (int i = 1; i < arg1.length(); i ++){
		if(arg1[i] == '0') arg2[i] = '1';
		else arg2 [i] = '0';
	}

	// if the least significant bit is 0, just change it to 1 and return
	if (arg2[arg1.length()-1] == '0'){
		arg2[arg1.length()-1] = '1';
		return;
	}

	// least significant bit is not 0
	// can't change the zero index, that represents the sign bit, j must be > 0.
	// normalizaiton also makes sure that adding 1 does not overflow
	for (int j = arg1.length()-1; j > 0; j --){											   
		if (arg2[j] == '0'){
			arg2[j] = '1';
			break;
		}
		arg2[j] = '0';
	}

	
}

//////////////////////////////////////////////////////////////

/////////////////////// Skip/Shift over zeroes //////////////
//arg: aq
//count: shift_count , will be decresing
//ch: skip over 0 or 1
int arg_skip_over(string & arg, int & count, char ch){
	int flag = 0;
	int skiped = 0;
	while (arg[1] == ch){
		skiped = 1;
		for (int i = 1; i < arg.length() -1; i++){
			arg[i] = arg[i+1];
		}
		arg[arg.length()-1] = ch;
		count--;
		total_cost += 3;
		
		// check if shift_count is already n+1	
		if (count == 0) {
			flag = 1;
			break;
		} 
		// we are done as far as SRT methods step count's concern
		
	}

	if(skiped){
		string des = "Shift Over ";
		stringstream ss;
		ss << ch;
		string temp;
		ss >> temp;
		des = des + temp + "'s";
		print_simulation_step(des, arg);
	}
	return (flag == 1)? 0 : 1 ;
}
/////////////////////////////////////////////////////////////

//////////////////////// Add function (if-else-logic) ///////////////////////
//arg1: content of aq
//arg2: b or -b 
/* addition logic
	Cin 	A 	B 		R 	Cout
	0 		0 	0 		0 	0
	0 		0 	1		1	0
	0 		1	0		1	0
	0		1	1		0	1
	1		0	0		1	0
	1		0	1		0	1
	1		1	0		0	1
	1		1	1		1	1		

*/
void arg_add(string & arg1, const string & arg2){
	char carry = '0';
	for (int i = arg2.length(); i >= 0; i--){
		if (arg1[i] == '*') continue;	// we don't perform addition on bit marked as *

			 if(carry == '0' && arg1[i] == '0' && arg2[i] == '0'){ arg1[i] = '0'; carry = '0'; }
		else if(carry == '0' && arg1[i] == '0' && arg2[i] == '1'){ arg1[i] = '1'; carry = '0'; }
		else if(carry == '0' && arg1[i] == '1' && arg2[i] == '0'){ arg1[i] = '1'; carry = '0'; }
		else if(carry == '0' && arg1[i] == '1' && arg2[i] == '1'){ arg1[i] = '0'; carry = '1'; }
		else if(carry == '1' && arg1[i] == '0' && arg2[i] == '0'){ arg1[i] = '1'; carry = '0'; }
		else if(carry == '1' && arg1[i] == '0' && arg2[i] == '1'){ arg1[i] = '0'; carry = '1'; }
		else if(carry == '1' && arg1[i] == '1' && arg2[i] == '0'){ arg1[i] = '0'; carry = '1'; }
		else if(carry == '1' && arg1[i] == '1' && arg2[i] == '1'){ arg1[i] = '1'; carry = '1'; }
		else{ /* do nothing, could also check for * marker here but not neccessarily.*/ }
	}
	// discard the final C_out

											// cost calculation for one addition (carry select)
	int m = 2;								// 2 bit full adder
	int N = arg2.length() - 1;				// total number of bits to add
	int m_t = 6;							// total time for m bit full adder
	int temp = (((N / m) - 1) * 2) + m_t;	// generalized equation for cost calculation in addition
	total_cost += temp;
}
/////////////////////////////////////////////////////////////

//////////////////////// shift-left-replace-q0 //////////////
// arg: aq
//ch: assign 0 or 1 to Q_0
void arg_shl_new_q(string & arg, char ch){
	for (int i = 1; i < arg.length() -1; i++){
		arg[i] = arg[i+1];
	}
	arg[arg.length()-1] = ch;
	total_cost += 3;	// one left shift operation costs 3 dt
}
//////////////////////////////////////////////////////////////

//////////////////////// SRT Method //////////////////////////

result division_srt(string aq, string b){
	result div_result;  			// result will be placed here and send all together
	int adjustment_factor = 0; 		// calculate how many bits to normalize
	int shift_count = b.length(); 	// algorithm ends after exactly no-of-bits(b) + 1 left shifts				 			
	string comp_b;  				// holds the 2's compliment of normalized b
	bool pos_result = true;			// flag to keep track whether previsious result was negative or not
	aq[0] = b[0] = '0'; 			// removing the dot (.) and will be used as carry bit

	//////////// SRT algorithm starts ////////////////////////////////////////////////////////////////////////////////////
	print_simulation_step(); //blank line
	print_simulation_step();
	print_simulation_step("Initializations", 20);
	print_simulation_step("A Q", aq, 0, true);
	print_simulation_step("B", b, 2);
	arg_normalize(b , adjustment_factor); 		// normalize b
	print_simulation_step("Normalize B", b, 2, true);
	arg_compliment(b, comp_b);					// calculates 2's compliment of normalized b
	print_simulation_step("2's Com. (Normalize B)", comp_b, 2, true);
	print_simulation_step("SRT Steps", 25);


	arg_normalize(aq, adjustment_factor, 1);	//adjust AQ, and insert * in LSBs
	print_simulation_step("Adjust AQ", aq);
	arg_skip_over(aq, shift_count, '0');			//shift over 0's and insert 0's in LSBs. left shift_count starts from here !
	//simulation step is inside the function
												// check if shift_count is equal n + 1, n = number of bit in b
	while (shift_count > 0){
												// substract B
		if (pos_result){ 
			arg_add(aq, comp_b);
			print_simulation_step("Subtract B", comp_b, 1);
		}	
												// check if result is positive or negetive
		if (aq[0] == '0'){
			print_simulation_step("Positive result", aq);
			arg_shl_new_q(aq, '1');				// result positive, shift AQ left, q_0 = 1
			shift_count--;
			print_simulation_step("Shift AQ left, q_0 <- 1", aq);
												// check if we already did n + 1 left shifts
			if(shift_count == 0){
				break;
			}
												// skip over zeroes
			if( ! arg_skip_over(aq, shift_count, '0')){
				break;
			}

			if (! pos_result){
				pos_result = true;
			}
		}
		else{
			pos_result = false;
			print_simulation_step("Negative result", aq);
			arg_shl_new_q(aq, '0');						// result negetive, shift AQ left, q_0 = 0
			shift_count--;
			print_simulation_step("Shift AQ left, q_0 <- 0", aq);
														// check if we already did n + 1 left shifts
			if(shift_count == 0){
				break;
			}
														// skip over 1's
			if( ! arg_skip_over(aq, shift_count, '1')){
				break;
			}
			arg_add(aq, b);								// add B to A
			print_simulation_step("Add B", b, 1);
		}
	}


	//////////////////////////////Remainder Correction///////////////////////////////
												// Remainder is negative, correct it.
	bool i_r = false;
	if(aq[0] == '1'){
		i_r = true;
		print_simulation_step("Remainder Correction", 20);
												// Right Shift A once
		for(int i = b.length()-1; i >0 ; i--){
			aq[i] = aq[i-1];
		}
		total_cost += 3; 						// one right shift while correcting the remainder, costs 3 dt
		print_simulation_step("RSH A", aq);					
		arg_add(aq, b);							// Add B to A
		print_simulation_step("Add B", b, 1);
	}
	//////Now AQ has the Remainder & Quotient, process the result/////////////////////
	//print_simulation_step("A Q", aq, 0, true); // original code
	print_simulation_step("A Q", aq, 0, i_r? false:true);
	aq[0] = b[0] = '.';														// all the calculaitons are done, change back to original form
	string quo = "." + aq.substr(b.length(), aq.length());					// get the quotient from Q
	int r_factor = 1;
	for (; (aq[r_factor] != '*') && (r_factor < b.length()) ; r_factor++);	// find the position of * in A
	string rmdr = aq.substr(0, r_factor);									// get temporary Remainder value
																			// Convert to actual value of Remainder
	for(int i = 0; i < aq.length() - r_factor; i++){
		rmdr.insert(1, "0");												// insert as many zeroes as needed
	}

	print_simulation_step("Results", 25);
	print_simulation_step("Remainder", rmdr, 2);
	print_simulation_step("Quotient", quo, 2);

	stringstream ss;
	ss << total_cost;
	string temp;
	ss >> temp;

	print_simulation_step("Total Cost", temp + " Δt", 2, false, true);
	print_simulation_step();
	print_simulation_step();
	//////////////////////////// Load the result into result object////////////////////
	div_result.remainder = rmdr;
	div_result.quotient = quo;
	div_result.execution_time = total_cost;
	total_cost = 0; // set the total cost count to zero again

	return div_result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// MAIN FUNCTION //////////////////////////////////////////////////////////////////


int main(int argc, char * argv[]){
	string arg_aq, arg_b;
	int overflow_type = 0;
	ifstream file("input.txt");
	if (!file.is_open()) {cout << "Can't open file !" << endl; exit(EXIT_FAILURE);}
	int iteration_count = 1;
	while (! file.eof()){
		file >> arg_aq >> arg_b;										// read input args from file

		cout << "\n(" << iteration_count++ << ")\n";
		////////////////Devide overflow testing/////////////////
		overflow_type = arg_check_overflow(arg_aq, arg_b);
		if (overflow_type == 1){
			cout << "\nA Q = " << arg_aq << "  B = " << arg_b << endl;
			cout << "Divide by Zero overflow occured! \n" << endl;
			continue;
		}
		else if(overflow_type == 2){
			cout << "\nA Q = " << arg_aq << "  B = " << arg_b << endl;
			cout << "A >= B overflow occured! \n" << endl;
			continue;
		}

		///////////////////////////////////////////////////////////

		result res = division_srt(arg_aq, arg_b);					// store the result in res.
		
		println(arg_aq, arg_b, res);

	}

	file.close();

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////