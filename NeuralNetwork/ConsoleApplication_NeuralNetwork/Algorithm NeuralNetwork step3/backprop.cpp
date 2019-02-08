/*
 * backprop.c
 * Backpropagation neural network library.
 *
 * 2016, April 7 - made Query return m_BiasVals, m_BHWeights and BIWeights
 * 2016, April 3 - cleaned up version for website publication
 * 1992 - originally written around this time
 * A note of credit:
 * This code had its origins as code obtained back around 1992 by sending
 * a floppy disk to The Amateur Scientist, Scientific American magazine.
 * I've since modified and added to it a great deal, and it's even on 
 * its 3rd OS (MS-DOS -> QNX -> Windows). As I no longer have the 
 * original I can't know how much is left to give credit for.
 */
#define CMDIFFSTEPSIZE   1 /* set to 1 for Chen & Mars differential step size */
#define DYNAMIC_LEARNING 0 /* set to 1 for Dynamic Learning */

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include "backprop.h"


/* The following sigmoid returns values from 0.0 to 1.0 */
#define sigmoid(x)           (1.0f / (1.0f + (float)exp(-(double)(x))))
#define sigmoidDerivative(x) ((float)(x)*(1.0f-(x))) 
/* random() for -1 to +1 */
#define random()             ((((float)rand()/(RAND_MAX)) * 2.0f) - 1.0f)
/* random() for -0.5 to +0.5
#define random()             (((float)rand()/(RAND_MAX)) - 0.5)
*/

/*
 * CreateNetwork - Create a new network with the given configuration.
 * Returns a pointer to the new network in 'n'.
 *
 * Return Value:
 * int  0: Success
 *     -1: Error, errno is set to:
 *         ENOMEM - out of memory
 *         EINVAL - config.Type is one which this server does not handle
 */
int KNeuralNetwork::CreateNetwork(KNeuralNetworkConfig *config)
{
   if (config->Type != BACKPROP_TYPE_NORMAL) {
      errno = EINVAL;
      return -1;
   }

   //if ((*n = (KNeuralNetwork *) malloc(sizeof(KNeuralNetwork))) == NULL) {
   //   errno = ENOMEM;
   //   return -1;
   //}
	
   m_NumInputs = config->m_NumInputs;
   m_NumHidden = config->m_NumHidden;
   m_NumOutputs = config->m_NumOutputs;
   m_NumConsecConverged = 0;
   m_Epoch = m_LastRMSError = m_RMSSquareOfOutputBetas = 0.0;
   m_NumBias = 1;
   if (config->m_StepSize == 0)
      m_StepSize = 0.5f;
   else
      m_StepSize = config->m_StepSize;
#if CMDIFFSTEPSIZE
   m_HStepSize = 0.1f * m_StepSize;
#endif
   if (config->m_Momentum == -1)
      m_Momentum = 0.5f;
   else
      m_Momentum = config->m_Momentum;
   m_Cost = config->m_Cost;
   m_GivenInputVals.resize(m_NumInputs);
   m_GivenDesiredOutputVals.resize(m_NumOutputs);
   m_IHWeights.resize( m_NumInputs * m_NumHidden );
   m_PrevDeltaIH.resize( m_NumInputs * m_NumHidden );
   m_PrevDeltaHO.resize( m_NumHidden * m_NumOutputs );
   m_PrevDeltaBH.resize( m_NumBias * m_NumHidden );
   m_PrevDeltaBO.resize( m_NumBias * m_NumOutputs );
   m_HiddenVals.resize( m_NumHidden );
   m_HiddenBetas.resize( m_NumHidden );
   m_HOWeights.resize( m_NumHidden * m_NumOutputs );
   m_BiasVals.resize( m_NumBias );
   m_BHWeights.resize( m_NumBias * m_NumHidden );
   m_BOWeights.resize( m_NumBias * m_NumOutputs );
   m_OutputVals.resize( m_NumOutputs );
   m_OutputBetas.resize( m_NumOutputs );

   _SetupAll();

   return 0;
}

/*
 * DestroyNetwork - Frees up any resources allocated for the
 * given neural network.
 *
 * Return Values:
 *    (none)
 */
void KNeuralNetwork::DestroyNetwork()
{
   //if (n == NULL)
   //   return;

   //if (m_GivenInputVals == NULL) return;
   ClearTraningSet();
   //free(m_GivenInputVals);
   m_GivenInputVals.clear();
   //if (m_GivenDesiredOutputVals != NULL) { free(m_GivenDesiredOutputVals); m_GivenDesiredOutputVals = NULL; }
   m_GivenDesiredOutputVals.clear();
   //if (m_IHWeights != NULL) { free(m_IHWeights); m_IHWeights = NULL; }
   m_IHWeights.clear();
   m_PrevDeltaIH.clear();
   m_PrevDeltaHO.clear();
   m_PrevDeltaBH.clear();
   m_PrevDeltaBO.clear();
   m_HiddenVals.clear();
   m_HiddenBetas.clear();
   m_HOWeights.clear();
   m_BiasVals.clear();
   m_BHWeights.clear();
   m_BOWeights.clear();
   m_OutputVals.clear();
   m_OutputBetas.clear();
   //m_GivenInputVals = NULL;
   //free(n);
}

/*
 * SetTrainingSet - Gives addresses of the input and target data 
 * in the form of a input values and output values. No data is copied
 * so do not destroy the originals until you call 
 * ClearTraningSet(), or DestroyNetwork().
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 */
int KNeuralNetwork::SetTrainingSet(int ntrainset, float* tinputvals, float* targetvals)
{
   //if (!n) {
   //   errno = ENOENT;
   //   return -1;
   //}

   ClearTraningSet();

   m_NumInTrainSet = ntrainset;
   m_TrainSetInputValsRef = tinputvals;
   m_TrainSetDesiredOutputValsRef = targetvals;
	
   return 0;
}

/*
 * ClearTraningSet - Invalidates the training set such that
 * you can no longer use it for training. After this you can free
 * up any memory associated with the training data you'd passed to
 * SetTrainingSet(). It has not been modified in any way.
 *
 * Return Values:
 *    (none)
 */
void KNeuralNetwork::ClearTraningSet()
{
   if (m_NumInTrainSet > 0) {
      m_TrainSetInputValsRef = NULL;
      m_TrainSetDesiredOutputValsRef = NULL;
      m_NumInTrainSet = 0;
   }
}

void KNeuralNetwork::_SetupAll()
{
   int i, h, o, b;
	
   m_InputReady = m_DesiredOutputReady = m_Learned = 0;

   m_LearningError = 0.0;
	
   for (i = 0;  i < m_NumInputs;  i++)
      m_GivenInputVals[i] = 0.0;
	
   for(h = 0;  h < m_NumHidden;  h++) {
      m_HiddenVals[h] = 0.0;
      for (i = 0;  i < m_NumInputs;  i++) {
         m_IHWeights[i+(h*m_NumInputs)] = random();
         m_PrevDeltaIH[i+(h*m_NumInputs)] = 0.0;
      }
      for (b = 0;  b < m_NumBias;  b++) {
         m_BHWeights[b+(h*m_NumBias)] = random();
         m_PrevDeltaBH[b+(h*m_NumBias)] = 0.0;
      }
   }

   for(o = 0;  o < m_NumOutputs;  o++) {
      m_OutputVals[o] = 0.0;
      for (h = 0;  h < m_NumHidden;  h++) {
         m_HOWeights[h+(o*m_NumHidden)] = random();
         m_PrevDeltaHO[h+(o*m_NumHidden)] = 0.0;
      }
      for (b = 0;  b < m_NumBias;  b++) {
         m_BOWeights[b+(o*m_NumBias)] = random();
         m_PrevDeltaBO[b+(o*m_NumBias)] = 0.0;
      }
   }
	
   for (b = 0;  b < m_NumBias;  b++)
      m_BiasVals[b] = 1.0;
}

/*
 * Learn - Tells backprop to learn the current training set ntimes.
 * The training set must already have been set by calling 
 * SetTrainingSet(). This does not return until the training
 * has been completed. You can call Query() after this to find out 
 * the results of the learning.
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 *        ESRCH if no SetTrainingSet() has been done yet.
 */
int KNeuralNetwork::Learn( int ntimes)
{
   int item, run;
	
   //if (!n) {
   //   errno = ENOENT;
   //   return -1;
   //}
   if (m_NumInTrainSet == 0) {
      errno = ESRCH;
      return -1;
   }

   for (run = 0;  run < ntimes;  run++) {
      for (item = 0;  item < m_NumInTrainSet;  item++) {
         /* set up for the given set item */
         m_InputValsRef = &(m_TrainSetInputValsRef[item*m_NumInputs]);
         m_DesiredOutputValsRef = &(m_TrainSetDesiredOutputValsRef[item*m_NumOutputs]);

         /* now do the learning */	
         _Forward();
         _Backward();
      }
	
      /* now that we have gone through the entire training set, calculate the
         RMS to see how well we have learned */
		   
      m_Epoch++;

      /* calculate the RMS error for the epoch just completed */
      m_LastRMSError = sqrt(m_RMSSquareOfOutputBetas / (m_NumInTrainSet * m_NumOutputs));
      m_RMSSquareOfOutputBetas = 0.0;
		
#if DYNAMIC_LEARNING
      if (m_Epoch > 1) {
         if (m_PrevRMSError < m_LastRMSError) {
            /* diverging */
            m_NumConsecConverged = 0;
            m_StepSize *= 0.95; /* make step size smaller */
#if CMDIFFSTEPSIZE
            m_HStepSize = 0.1 * m_StepSize;
#endif
#ifdef DISPLAYMSGS
            printf("m_Epoch: %d Diverging:  Prev %f, New %f, Step size %f\n",
               m_Epoch, m_PrevRMSError, m_LastRMSError, m_StepSize);
#endif
         } else if (m_PrevRMSError > m_LastRMSError) {
            /* converging */
            m_NumConsecConverged++;
            if (m_NumConsecConverged == 5) {
               m_StepSize += 0.04; /* make step size bigger */
#if CMDIFFSTEPSIZE
               m_HStepSize = 0.1 * m_StepSize;
#endif
#ifdef DISPLAYMSGS
               printf("m_Epoch: %d Converging: Prev %f, New %f, Step size %f\n",
                  m_Epoch, m_PrevRMSError, m_LastRMSError, m_StepSize);
#endif
               m_NumConsecConverged = 0;
            }
         } else {
            m_NumConsecConverged = 0;
         }
      }
      m_PrevRMSError = m_LastRMSError;
#endif
   }
   m_Learned = 1;
   return 0;
}

/*
 * Evaluate - Evaluate but don't learn the current input set.
 * This is usually preceded by a call to SetInput() and is
 * typically called after the training set (epoch) has been learned.
 *
 * If you give eoutputvals as NULL then you can do a Query() to 
 * get the results.
 *
 * If you give the address of a buffer to return the results of the
 * evaluation (eoutputvals != NULL) then the results will copied to the 
 * eoutputvals buffer.
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 *        ESRCH if no SetInput() has been done yet.
 *        ENODEV if both CreateNetwork() and SetInput()
 *               have been done but bkp_earn() has not been done
 *               yet (ie; neural net has not had any training).
 *        EINVAL if sizeofoutputvals is not the same as the
 *               size understood according to n. This is to help
 *               prevent buffer overflow during copying.
 */
int KNeuralNetwork::Evaluate(float *eoutputvals, int sizeofoutputvals)
{
   //if (!n) {
   //   errno = ENOENT;
   //   return -1;
   //}
   if (!m_InputReady) {
      errno = ESRCH;
      return -1;
   }
   if (!m_Learned) {
      errno = ENODEV;
      return -1;
   }

   m_InputValsRef = &m_GivenInputVals[0];
   m_DesiredOutputValsRef = &m_GivenDesiredOutputVals[0];

   _Forward();

   if (eoutputvals) {
      if (sizeofoutputvals != m_NumOutputs*sizeof(float)) {
         errno = EINVAL;
         return -1;
      }
      memcpy(eoutputvals, &m_OutputVals[0], m_NumOutputs*sizeof(float));
   }
   return 0;
}

/*
 * _Forward - This makes a pass from the input units to the hidden 
 * units to the output units, updating the hidden units, output units and 
 * other components. This is how the neural network is run in order to
 * evaluate a set of input values to get output values.
 * When training the neural network, this is the first step in the
 * backpropagation algorithm.
 */
void KNeuralNetwork::_Forward()
{
   int i, h, o, b;
	
   m_LearningError = 0.0;

   /*
    * Apply input unit values to weights between input units and hidden units
    * Apply bias unit values to weights between bias units and hidden units
    */
   for (h = 0;  h < m_NumHidden;  h++) {
      m_HiddenVals[h] = 0.0;
      m_HiddenBetas[h] = 0.0; /* needed if doing a backward pass later */
      for (i = 0;  i < m_NumInputs;  i++)
         m_HiddenVals[h] = m_HiddenVals[h] + (m_InputValsRef[i] * m_IHWeights[i+(h*m_NumInputs)]);
      for (b = 0;  b < m_NumBias;  b++)
         m_HiddenVals[h] = m_HiddenVals[h] + (m_BiasVals[b] * m_BHWeights[b+(h*m_NumBias)]);
      m_HiddenVals[h] = sigmoid(m_HiddenVals[h]);
   }
	
   /*
    * Apply hidden unit values to weights between hidden units and outputs
    * Apply bias unit values to weights between bias units and outputs
    */
   for (o = 0;  o < m_NumOutputs;  o++) {
      m_OutputVals[o] = 0.0;
      for (h = 0;  h < m_NumHidden;  h++)
         m_OutputVals[o] = m_OutputVals[o] + (m_HiddenVals[h] * m_HOWeights[h+(o*m_NumHidden)]);
      for (b = 0;  b < m_NumBias;  b++)
         m_OutputVals[o] = m_OutputVals[o] + (m_BiasVals[b] * m_BOWeights[b+(o*m_NumBias)]);
      m_OutputVals[o] = sigmoid(m_OutputVals[o]);
      m_LearningError = m_LearningError + 
         ((m_OutputVals[o] - m_DesiredOutputValsRef[o]) * (m_OutputVals[o] - m_DesiredOutputValsRef[o]));
   }
   m_LearningError = m_LearningError / 2.0f;
}

/*
 * _Backward - This is the 2nd half of the backpropagation algorithm
 * which is carried out immediately after _Forward() has done its
 * step of calculating the outputs. This does the reverse, comparing
 * those output values to those given as targets in the training set 
 * and updating the weights and other components appropriately, which
 * is essentially the training of the neural network.
 */
void KNeuralNetwork::_Backward()
{
   float deltaweight;
   int i, h, o, b;

   for (o = 0;  o < m_NumOutputs;  o++) {
      /* calculate beta for output units */
      m_OutputBetas[o] = m_DesiredOutputValsRef[o] - m_OutputVals[o];

      /* update for RMS error */
      m_RMSSquareOfOutputBetas += (m_OutputBetas[o] * m_OutputBetas[o]);

      /* update hidden to output weights */
      for (h = 0;  h < m_NumHidden;  h++) {
         /* calculate beta for hidden units for later */
         m_HiddenBetas[h] = m_HiddenBetas[h] +
            (m_HOWeights[h+(o*m_NumHidden)] * sigmoidDerivative(m_OutputVals[o]) * m_OutputBetas[o]);

#if CMDIFFSTEPSIZE
         deltaweight = m_HiddenVals[h] * m_OutputBetas[o];
#else
         deltaweight = m_HiddenVals[h] * m_OutputBetas[o] *
            sigmoidDerivative(m_OutputVals[o]);
#endif
         m_HOWeights[h+(o*m_NumHidden)] = m_HOWeights[h+(o*m_NumHidden)] + 
            (m_StepSize * deltaweight) +
            (m_Momentum * m_PrevDeltaHO[h+(o*m_NumHidden)]);
         m_PrevDeltaHO[h+(o*m_NumHidden)] = deltaweight;
      }
      /* update bias to output weights */
      for (b = 0;  b < m_NumBias;  b++) {
#if CMDIFFSTEPSIZE
         deltaweight = m_BiasVals[b] * m_OutputBetas[o];
#else
         deltaweight = m_BiasVals[b] * m_OutputBetas[o] +
            sigmoidDerivative(m_OutputVals[o]);
#endif
         m_BOWeights[b+(o*m_NumBias)] = m_BOWeights[b+(o*m_NumBias)] +
            (m_StepSize * deltaweight) +
            (m_Momentum * m_PrevDeltaBO[b+(o*m_NumBias)]);
         m_PrevDeltaBO[b+(o*m_NumBias)] = deltaweight;
      }
   }

   for (h = 0;  h < m_NumHidden;  h++) {
      /* update input to hidden weights */
      for (i = 0;  i < m_NumInputs;  i++) {
         deltaweight = m_InputValsRef[i] * sigmoidDerivative(m_HiddenVals[h]) *
            m_HiddenBetas[h];
         m_IHWeights[i+(h*m_NumInputs)] = m_IHWeights[i+(h*m_NumInputs)] + 
#if CMDIFFSTEPSIZE
            (m_HStepSize * deltaweight) +
#else
            (m_StepSize * deltaweight) +
#endif
            (m_Momentum * m_PrevDeltaIH[i+(h*m_NumInputs)]);
         m_PrevDeltaIH[i+(h*m_NumInputs)] = deltaweight;
         if (m_Cost)
            m_IHWeights[i+(h*m_NumInputs)] = m_IHWeights[i+(h*m_NumInputs)] - 
               (m_Cost * m_IHWeights[i+(h*m_NumInputs)]);
      }
      /* update bias to hidden weights */
      for (b = 0;  b < m_NumBias;  b++) {
         deltaweight = m_BiasVals[b] * m_HiddenBetas[h] *
            sigmoidDerivative(m_HiddenVals[h]);
         m_BHWeights[b+(h*m_NumBias)] = m_BHWeights[b+(h*m_NumBias)] +
#if CMDIFFSTEPSIZE
            (m_HStepSize * deltaweight) +
#else
            (m_StepSize * deltaweight) +
#endif
            (m_Momentum * m_PrevDeltaBH[b+(h*m_NumBias)]);
         m_PrevDeltaBH[b+(h*m_NumBias)] = deltaweight;
         if (m_Cost)
            m_BHWeights[b+(h*m_NumBias)] = m_BHWeights[b+(h*m_NumBias)] - 
               (m_Cost * m_BHWeights[b+(h*m_NumBias)]);
      }
   }
}

/*
 * Query - Get the current state of the neural network.
 *
 * Parameters (all parameters return information unless given as NULL):
 * float *qlastlearningerror: The error for the last set of inputs
 *                            and outputs learned by Learn()
 *                            or evaluated by Evaluate().
 *                            It is the sum of the squares
 *                            of the difference between the actual
 *                            outputs and the target or desired outputs,
 *                            all divided by 2 
 * float *qlastrmserror:      The RMS error for the last epoch learned
 *                            i.e. the learning of the training set.
 * float *qinputvals:         An array to fill with the current input 
 *                            values (must be at least 
 *                            KNeuralNetworkConfig.m_NumInputs * sizeof(float))
 * float *qihweights:         An array to fill with the current input
 *                            units to hidden units weights (must be at 
 *                            least KNeuralNetworkConfig.m_NumInputs * 
 *                            KNeuralNetworkConfig.m_NumHidden * sizeof(float)
 * float *qhiddenvals:        An array to fill with the current hidden
 *                            unit values (must be at least 
 *                            KNeuralNetworkConfig.m_NumHidden * sizeof(float))
 * float *qhoweights:         An array to fill with the current hidden
 *                            units to output units weights (must be at 
 *                            least KNeuralNetworkConfig.m_NumHidden * 
 *                            KNeuralNetworkConfig.m_NumOutputs * sizeof(float))
 * float *qoutputvals:        An array to fill with the current output
 *                            values (must be at least 
 *                            KNeuralNetworkConfig.m_NumOutputs * sizeof(float))
 * Note that for the following three, the size required is 1 * ...
 * The reason for the 1 is because there is only one bias unit for
 * everything. Theoretically there could be more though.
 * float *qbhweights:         An array to fill with the current bias
 *                            units to hidden units weights (must be at 
 *                            least 1 * KNeuralNetworkConfig->m_NumHidden * 
 *                            sizeof(float))
 * float *qbiasvals:          An array to fill with the current bias
 *                            values (must be at least 1 * sizeof(float))
 * float *qboweights:         An array to fill with the current bias
 *                            units to output units weights (must be at
 *                            least 1 * (*n)->m_NumOutputs * sizeof(float))
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 *        ENODEV if CreateNetwork() has been done
 *               but Learn() has not been done yet (ie; neural 
 *               net has not had any training).
 */
int KNeuralNetwork::Query(float *qlastlearningerror, float *qlastrmserror,
	float *qinputvals, float *qihweights, float *qhiddenvals, 
	float *qhoweights, float *qoutputvals,
      float *qbhweights, float *qbiasvals, float *qboweights)
{
   if (!m_Learned) {
      errno  = ENODEV;
      return -1;
   }
   if (qlastlearningerror)
      *qlastlearningerror = m_LearningError;
   if (qlastrmserror)
      *qlastrmserror = m_LastRMSError;
   if (qinputvals)
      memcpy(qinputvals, m_InputValsRef, m_NumInputs*sizeof(float));
   if (qihweights)
      memcpy(qihweights, &m_IHWeights[0], (m_NumInputs*m_NumHidden)*sizeof(float));
   if (qhiddenvals)
       memcpy( qhiddenvals, &m_HiddenVals[0], m_NumHidden*sizeof(float) );
   if (qhoweights)
       memcpy( qhoweights, &m_HOWeights[0], (m_NumHidden*m_NumOutputs)*sizeof(float) );
   if (qoutputvals)
       memcpy( qoutputvals, &m_OutputVals[0], m_NumOutputs*sizeof(float) );
   if (qbhweights)
       memcpy( qbhweights, &m_BHWeights[0], m_NumBias*m_NumHidden*sizeof(float) );
   if (qbiasvals)
       memcpy( qbiasvals, &m_BiasVals[0], m_NumBias*sizeof(float) );
   if (qboweights)
       memcpy( qboweights, &m_BOWeights[0], m_NumBias*m_NumOutputs*sizeof(float) );
   return 0;
}

/*
 * SetInput - Use this to set the current input values of the neural
 * network. Nothing is done with the values until Learn() is called.
 *
 * Parameters:
 * int setall: If 1: Set all inputs to Val. Any sinputvals are ignored so 
 *                   you may as well give sinputvals as NULL.
 * float val: See SetAll.
 * float sinputvals: An array of input values.  The array should contain
 *                   KNeuralNetworkConfig.m_NumInputs elements.
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 */
int KNeuralNetwork::SetInput(int setall, float val, float *sinputvals)
{
   int i;

   //if (!n) {
   //   errno = ENOENT;
   //   return -1;
   //}

   if (setall) {
      for (i = 0;  i < m_NumInputs;  i++)
         m_GivenInputVals[i] = val;
   } else {
      memcpy(&m_GivenInputVals[0], sinputvals, m_NumInputs*sizeof(float));
   }

   m_InputReady = 1;
   return 0;
}

/*
 * SetOutput - Use this so that Evaluate() can calculate the
 * error between the output values you passs to SetOutput() and 
 * the output it gets by evaulating the network using the input values
 * you passed to the last call to SetInput(). The purpose is so
 * that you can find out what that error is using Query()'s
 * qlastlearningerror argument. Typically SetOutput() will have been
 * accompanied by a call to SetInput().
 *
 * Parameters:
 * int setall: If 1: Set all outputs to val. Any soutputvals 
 *                   are ignored so you may as well give 
 *                   soutputvals as NULL.
 *             If 0: val is ignored. You must provide soutputvals.
 * float val:  See setall.
 * float sonputvals: An array of input values.  The array should contain
 *                   KNeuralNetworkConfig.m_NumInputs elements.
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 */
int KNeuralNetwork::SetOutput(int setall, float val, float *soutputvals)
{
   int i;

   //if (!n) {
   //   errno = ENOENT;
   //   return -1;
   //}

   if (setall) {
      for (i = 0;  i < m_NumOutputs;  i++)
         m_GivenDesiredOutputVals[i] = val;
   } else {
      memcpy(&m_GivenDesiredOutputVals[0], soutputvals, m_NumOutputs*sizeof(float));
   }

   m_DesiredOutputReady = 1;
   return 0;
}

/*
 * LoadFromFile - Creates a neural network using the information
 * loaded from the given file and returns a pointer to it in n.
 * If successful, the end result of this will be a neural network
 * for which CreateNetwork() will effectively have been done.
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        EOK or any applicable errors from the open() or read() 
 *        functions or ENOMEM if no memory.
 */
int KNeuralNetwork::LoadFromFile(char *fname)
{
   int fd, returncode;
   KNeuralNetworkConfig config;
	
   returncode = -1;
	
   config.m_NumInputs = m_NumInputs;
   config.m_NumHidden = m_NumHidden;
   config.m_NumOutputs = m_NumOutputs;
   config.m_StepSize = m_StepSize;
   config.m_Momentum = m_Momentum;
   config.m_Cost = m_Cost;

   if (CreateNetwork(&config) == -1)
      return returncode;
	
   if ((fd = _open(fname, O_RDONLY)) == -1)
      return returncode;

   m_InputValsRef = &m_GivenInputVals[0];
   m_DesiredOutputValsRef = &m_GivenDesiredOutputVals[0];

   if (_read(fd, (int *) &m_NumInputs, sizeof(int)) == -1)
      goto errandret;
   if( _read( fd, (int *)&m_NumHidden, sizeof( int ) ) == -1 )
      goto errandret;
   if( _read( fd, (int *)&m_NumOutputs, sizeof( int ) ) == -1 )
      goto errandret;
   if( _read( fd, (int *)&m_NumBias, sizeof( int ) ) == -1 )
      goto errandret;
		
   if( _read( fd, (int *)&m_InputReady, sizeof( int ) ) == -1 )
      goto errandret;
   if( _read( fd, (int *)&m_DesiredOutputReady, sizeof( int ) ) == -1 )
      goto errandret;
   if( _read( fd, (int *)&m_Learned, sizeof( int ) ) == -1 )
      goto errandret;
		
   if( _read( fd, m_InputValsRef, m_NumInputs * sizeof( float ) ) == -1 )
      goto errandret;
   if( _read( fd, m_DesiredOutputValsRef, m_NumOutputs * sizeof( float ) ) == -1 )
      goto errandret;
   if( _read( fd, &m_IHWeights[0], m_NumInputs * m_NumHidden * sizeof( float ) ) == -1 )
      goto errandret;
   if( _read( fd, &m_PrevDeltaIH[0], m_NumInputs * m_NumHidden * sizeof( float ) ) == -1 )
      goto errandret;
   if( _read( fd, &m_PrevDeltaHO[0], m_NumHidden * m_NumOutputs * sizeof( float ) ) == -1 )
      goto errandret;
   if( _read( fd, &m_PrevDeltaBH[0], m_NumBias * m_NumHidden * sizeof( float ) ) == -1 )
      goto errandret;
   if (_read( fd, &m_PrevDeltaBO[0], m_NumBias * m_NumOutputs * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_HiddenVals[0], m_NumHidden * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_HiddenBetas[0], m_NumHidden * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_HOWeights[0], m_NumHidden * m_NumOutputs * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_BiasVals[0], m_NumBias * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_BHWeights[0], m_NumBias * m_NumHidden * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_BOWeights[0], m_NumBias * m_NumOutputs * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_OutputVals[0], m_NumOutputs * sizeof(float) ) == -1)
      goto errandret;
   if (_read( fd, &m_OutputBetas[0], m_NumOutputs * sizeof(float) ) == -1)
      goto errandret;
		
   returncode = 0;
   goto cleanupandret;

errandret:
   DestroyNetwork();

cleanupandret:
   _close(fd);
	
   return returncode;
}	

/*
 * SaveToFile
 *
 * The format of the file is:
 *
 *  1. Number of inputs (sizeof(int))
 *  2. Number of hidden units (sizeof(int))
 *  3. Number of outputs (sizeof(int))
 *  4. Number of bias units (sizeof(int))
 *  5. Is input ready? 0 = no, 1 = yes (sizeof(int))
 *  6. Is desired output ready? 0 = no, 1 = yes (sizeof(int))
 *  7. Has some learning been done? 0 = no, 1 = yes (sizeof(int))
 *  8. Current input values (m_InputValsRef) (m_NumInputs * sizeof(float))
 *  9. Current desired output values (m_DesiredOutputValsRef) (m_NumOutputs * sizeof(float))
 * 10. Current input-hidden weights (m_IHWeights) (m_NumInputs * m_NumHidden * sizeof(float))
 * 11. Previous input-hidden weight deltas (m_PrevDeltaIH) (m_NumInputs * m_NumHidden * sizeof(float))
 * 12. Previous output-hidden weight deltas (m_PrevDeltaHO) (m_NumHidden * m_NumOutputs * sizeof(float))
 * 13. Previous bias-hidden weight deltas (m_PrevDeltaBH) (m_NumBias * m_NumHidden * sizeof(float))
 * 14. Previous bias-output weight deltas (m_PrevDeltaBO) (m_NumBias * m_NumOutputs * sizeof(float))
 * 15. Current hidden unit values (m_HiddenVals) (m_NumHidden * sizeof(float))
 * 16. Current hidden unit beta values (m_HiddenBetas) (m_NumHidden * sizeof(float))
 * 17. Current hidden-output weights (m_HOWeights) (m_NumHidden * m_NumOutputs * sizeof(float))
 * 18. Current bias unit values (m_BiasVals) (m_NumBias * sizeof(float))
 * 19. Current bias-hidden weights (m_BHWeights) (m_NumBias * m_NumHidden * sizeof(float))
 * 20. Current bias-output weights (m_BOWeights) (m_NumBias * m_NumOutputs * sizeof(float))
 * 21. Current output values (m_OutputVals) (m_NumOutputs * sizeof(float))
 * 22. Current output unit betas (m_OutputBetas) (m_NumOutputs * sizeof(float))
 *
 * Return Values:
 * int 0: Success
 *    -1: Error, errno is:
 *        ENOENT if no CreateNetwork() has been done yet.
 *        EOK or any applicable errors from the open() or write() 
 *        functions.
 */
int KNeuralNetwork::SaveToFile(char *fname)
{
   int fd, returncode;
	
   returncode = -1;
	
   fd = _open(fname, O_WRONLY | O_CREAT | O_TRUNC/*, 
         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH*/);
   if (fd == -1)
      return returncode;
	
   if (_write(fd, (int *) &m_NumInputs, sizeof(int)) == -1)
      goto cleanupandret;
   if( _write( fd, (int *)&m_NumHidden, sizeof( int ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, (int *)&m_NumOutputs, sizeof( int ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, (int *)&m_NumBias, sizeof( int ) ) == -1 )
      goto cleanupandret;
		
   if( _write( fd, (int *)&m_InputReady, sizeof( int ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, (int *)&m_DesiredOutputReady, sizeof( int ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, (int *)&m_Learned, sizeof( int ) ) == -1 )
      goto cleanupandret;
		
   if( _write( fd, m_InputValsRef, m_NumInputs * sizeof( float ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, m_DesiredOutputValsRef, m_NumOutputs * sizeof( float ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, &m_IHWeights[0], m_NumInputs * m_NumHidden * sizeof( float ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, &m_PrevDeltaIH[0], m_NumInputs * m_NumHidden * sizeof( float ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, &m_PrevDeltaHO[0], m_NumHidden * m_NumOutputs * sizeof( float ) ) == -1 )
      goto cleanupandret;
   if( _write( fd, &m_PrevDeltaBH[0], m_NumBias * m_NumHidden * sizeof( float ) ) == -1 )
      goto cleanupandret;
   if (_write( fd, &m_PrevDeltaBO[0], m_NumBias * m_NumOutputs * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_HiddenVals[0], m_NumHidden * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_HiddenBetas[0], m_NumHidden * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_HOWeights[0], m_NumHidden * m_NumOutputs * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_BiasVals[0], m_NumBias * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_BHWeights[0], m_NumBias * m_NumHidden * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_BOWeights[0], m_NumBias * m_NumOutputs * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_OutputVals[0], m_NumOutputs * sizeof(float) ) == -1)
      goto cleanupandret;
   if (_write( fd, &m_OutputBetas[0], m_NumOutputs * sizeof(float) ) == -1)
      goto cleanupandret;
		
   returncode = 0;
		
cleanupandret:
   _close(fd);
	
   return returncode;
}
