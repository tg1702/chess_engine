#ifndef NNUE_H
#define NNUE_H

#include <iostream>
#include "types.h"

const int INPUT_SIZE = 768;
const int HL_SIZE = 3072;

const int SCALE = 400;
const int QA = 255;
const int QB = 64;
const int MAX_TRAINING_INPUTS = 100;
const int MAX_TRAINING_OUTPUTS = 100;

struct Accumulator{
	int16_t values[HL_SIZE] = {0};

};

struct AccumulatorPair{
	Accumulator white;
	Accumulator black;
};

class NNUE{
	public:
		NNUE(){
			
			output_bias = 0;
			learning_rate = 0.1;
			num_epochs = 1000;

			for (int i = 0; i < INPUT_SIZE; i++){
				for (int j = 0; j < HL_SIZE; j++){
					accumulator_weights[i][j] = 0;
					accumulator_biases[j] = 0;
				}
			}
		}




		void fit(int16_t[INPUT_SIZE][MAX_TRAINING_INPUTS], int16_t[MAX_TRAINING_OUTPUTS]);
		void accumulatorAdd(Accumulator&, size_t);
		void accumulatorSub(Accumulator&, size_t);
		int16_t CReLU(int16_t, int16_t, int16_t);
		int16_t derivative_CReLU(int16_t, int16_t, int16_t);
		int32_t activation(int16_t);
		int32_t activation_derivative(int16_t);
		int32_t forward(Accumulator&, Accumulator&);
		void backprop(int16_t[INPUT_SIZE], int16_t);
		void predict(Accumulator&, Accumulator&);
	private:
		int16_t accumulator_weights[INPUT_SIZE][HL_SIZE];
		int16_t accumulator_biases[HL_SIZE];
		int16_t output_weights[2 * HL_SIZE];
		int16_t output_bias;
		int16_t learning_rate;
		int num_epochs;
};

void NNUE::accumulatorAdd(Accumulator& accumulator, size_t index){
	for (int i = 0; i < HL_SIZE; i++){
		accumulator.values[i] += accumulator_weights[index][i];

	}
}

void NNUE::accumulatorSub(Accumulator& accumulator, size_t index){
	for (int i = 0; i < HL_SIZE; i++){
		accumulator.values[i] -= accumulator_weights[index][i];
	}
}

int16_t NNUE::CReLU(int16_t value, int16_t min, int16_t max){
	if (value <= min)
		return min;

	if (value >= max)
		return max;

	return value;

}

int16_t NNUE::derivative_CReLU(int16_t value, int16_t min, int16_t max){
	if (value <= min || value >= max)
		return 0;

	return 1;

}

int32_t NNUE::activation(int16_t value){
	return CReLU(value, 0, QA);
}

int32_t NNUE::activation_derivative(int16_t value){
	return derivative_CReLU(value, 0, QA);
}

int32_t NNUE::forward(Accumulator& side_to_move_acc, Accumulator& not_side_to_move_acc){
	int32_t eval = 0;

	for (int i = 0; i < HL_SIZE; i++){
		eval += activation(side_to_move_acc.values[i]) * output_weights[i];
		eval += activation(not_side_to_move_acc.values[i]) * output_weights[i + HL_SIZE];
	}	


	eval += output_bias;
	eval *= SCALE;
	eval /= QA * QB;

	return eval;

}

void NNUE::backprop(int16_t inputs[INPUT_SIZE], int16_t training_output){
	Accumulator side_to_move_acc;
	Accumulator not_side_to_move_acc;

	// Forward pass
	int32_t eval = 0;

	for (int i = 0; i < HL_SIZE; i++){
		for (int j = 0; j < INPUT_SIZE; j++){
			side_to_move_acc.values[i] += inputs[j] * accumulator_weights[j][i];
			not_side_to_move_acc.values[i] += inputs[j] * accumulator_weights[j][i];
		}

		side_to_move_acc.values[i] += accumulator_biases[i];
		not_side_to_move_acc.values[i] += accumulator_biases[i];
	}
	
	

	for (int i = 0; i < HL_SIZE; i++){
		eval += activation(side_to_move_acc.values[i]) * output_weights[i];
		eval += activation(not_side_to_move_acc.values[i]) * output_weights[i + HL_SIZE];

		
	}	


	eval += output_bias;
	eval *= SCALE;
	eval /= QA * QB;

	
	

	// input -> hidden : y = (Input_1 * w_1 + b_1 + Input_1 * w_2 + b_2) +
	//  hidden -> output: ()
	
	//Backward pass

	int32_t error = (training_output - eval);
	
	
	

	// calculating error in hidden layer
	for (int i = 0; i < HL_SIZE; i++){
		int32_t grad_output_weight_stm = error * activation(side_to_move_acc.values[i]);
		int32_t grad_output_weight_nstm = error * activation(not_side_to_move_acc.values[i]);

		output_weights[i] += learning_rate * grad_output_weight_stm;
		output_weights[i + HL_SIZE] += learning_rate * grad_output_weight_nstm;
	}

	output_bias += learning_rate * error;

	

	for (int i = 0; i < HL_SIZE; i++){
		int32_t hidden_error_stm = error * output_weights[i] * activation_derivative(side_to_move_acc.values[i]);
		int32_t hidden_error_nstm = error * output_weights[i] * activation_derivative(not_side_to_move_acc.values[i]);
		for (int j = 0; j < INPUT_SIZE; j++){
			accumulator_weights[j][i] += learning_rate * hidden_error_stm * inputs[j];
			accumulator_weights[j][i] += learning_rate * hidden_error_nstm * inputs[j];

		}

		accumulator_biases[i] += learning_rate * hidden_error_stm;
		accumulator_biases[i] += learning_rate * hidden_error_nstm;
	}


}

void NNUE::fit(int16_t inputs[INPUT_SIZE][MAX_TRAINING_INPUTS], int16_t evals[MAX_TRAINING_OUTPUTS]){

	for (int i = 0; i < num_epochs; i++){
		for (int j = 0; j < MAX_TRAINING_INPUTS; j++){
			
			int16_t input[INPUT_SIZE];

			for (int k = 0; k < INPUT_SIZE; k++){
				input[k] = inputs[k][j];
			}
			
			backprop(input, evals[j]);
			std::cout << "Epoch " << i << " completed\n";
		}
		
	}	
}

void NNUE::predict(Accumulator& side_to_move_acc, Accumulator& not_side_to_move_acc){
	forward(side_to_move_acc, not_side_to_move_acc);

}

#endif