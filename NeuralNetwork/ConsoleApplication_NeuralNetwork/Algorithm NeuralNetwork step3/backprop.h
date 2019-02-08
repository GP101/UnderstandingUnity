/*
 * backprop.h
 * Backpropagation neural network library.
 */
#ifndef BACKPROPHDR
#define BACKPROPHDR

#include <vector>

#define BACKPROP_TYPE_NORMAL 1

/*
* KNeuralNetworkConfig
*/
struct KNeuralNetworkConfig
{
    short Type;           /* see BACKPROP_TYPE_* above,
                          currently only BACKPROP_TYPE_NORMAL exists */
    int m_NumInputs;        /* number of input units */
    int m_NumHidden;        /* number of hidden units */
    int m_NumOutputs;       /* number of output units */
    float m_StepSize;       /* step size (aka learning rate) for changing
                          the weights between units, it is > 0 and
                          < 1 (defaults to 0.5 if given 0) */
    float m_Momentum;       /* helps prevent getting stuck in local
                          minimas, it is a value betwene 0 - 1,
                          use a small m_StepSize when using a large
                          m_Momentum (defaults to 0.5 if given -1) */
    float m_Cost;           /* fraction of the weight's own value to
                          subtract from itself each time the weight
                          is modified, use 0 if not desired */
};

/*
 * KNeuralNetwork
 */
class KNeuralNetwork
{
public:
   int m_NumInTrainSet;
   int m_NumInputs;
   int m_NumHidden;
   int m_NumOutputs;
   int m_NumBias;
   float* m_TrainSetDesiredOutputValsRef; /* values set by SetTrainingSet() */
   std::vector<float>   m_GivenDesiredOutputVals; /* values set by bkp_set_output() */
   float* m_DesiredOutputValsRef;         /* pointer used by _Forward() and _Backward() */
   float* m_TrainSetInputValsRef;         /* values set by SetTrainingSet() */
   std::vector<float>   m_GivenInputVals; /* values set by bkp_set_input() */
   float* m_InputValsRef;                 /* pointer used by _Forward() and _Backward() */ // reference
   std::vector<float>   m_IHWeights;/* NumInputs x NumHidden */
   std::vector<float>   m_PrevDeltaIH;
   std::vector<float>   m_PrevDeltaHO;
   std::vector<float>   m_PrevDeltaBH;
   std::vector<float>   m_PrevDeltaBO;
   std::vector<float>   m_HiddenVals;
   std::vector<float>   m_HiddenBetas;
   std::vector<float>   m_HOWeights; /* m_NumHidden x m_NumOutputs */
   std::vector<float>   m_BiasVals;
   std::vector<float>   m_BHWeights; /* m_NumBias x m_NumHidden */
   std::vector<float>   m_BOWeights; /* m_NumBias x m_NumOutputs */
   std::vector<float>   m_OutputVals;
   std::vector<float>   m_OutputBetas;

   float m_RMSSquareOfOutputBetas;
   float m_PrevRMSError;
   float m_LastRMSError;
   float m_LearningError;
   float m_StepSize;
   float m_HStepSize;
   float m_Momentum;
   float m_Cost;
   int m_Epoch;
   int m_NumConsecConverged;

   int m_InputReady;
   int m_DesiredOutputReady;
   int m_Learned;

public:
    int CreateNetwork(KNeuralNetworkConfig *config);
    void DestroyNetwork();
    int SetTrainingSet(int ntrainset, float *tinputvals, float *targetvals);
    void ClearTraningSet();
    int Learn(int ntimes);
    int Evaluate(float *eoutputvals, int sizeofoutputvals);
    int Query( float *qlastlearningerror, float *qlastrmserror
        , float *qinputvals, float *qihweights, float *qhiddenvals
        , float *qhoweights, float *qoutputvals
        , float *qbhweights, float *qbiasvals, float *qboweights);
    int SetInput(int setall, float val, float *sinputvals);
    int SetOutput(int setall, float val, float *soutputvals);
    int LoadFromFile(char *fname);
    int SaveToFile(char *fname);

private:
    void _SetupAll();
    void _Forward();
    void _Backward();
};

#endif
