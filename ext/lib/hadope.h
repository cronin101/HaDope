#ifndef HADOPE_H
#define HADOPE_H
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif

typedef struct {
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queue;
} HadopeEnvironment;

typedef struct {
    cl_device_id cpu_device_id;
    cl_device_id gpu_device_id;
    cl_context context;
    cl_command_queue cpu_queue;
    cl_command_queue gpu_queue;
} HadopeHybridEnvironment;

typedef struct {
    cl_kernel kernel;
    cl_program program;
    char* name;
} HadopeTask;

typedef enum {
    INTEGER_BUFFER,
    DOUBLE_BUFFER
} buffer_contents_type;

typedef struct {
    int buffer_entries;
    cl_mem buffer;
    buffer_contents_type type;
} HadopeMemoryBuffer;

void createHadopeEnvironment(
    const cl_device_type device_type,
    HadopeEnvironment* env
);

void createHadopeHybridEnvironment(
    HadopeHybridEnvironment* env
);

cl_mem createMemoryBuffer(
    const HadopeEnvironment* env,
    const size_t required_memory,
    const cl_mem_flags type
);

void pinArrayForDevice(
    const HadopeEnvironment* env,
    void* dataset,
    int dataset_length,
    size_t dataset_size,
    HadopeMemoryBuffer* result,
    buffer_contents_type type
);

void buildTaskFromSource(
    const HadopeEnvironment* env,
    const char* kernel_source,
    const char* name,
    HadopeTask* result
);

void loadIntArrayIntoDevice(
    const HadopeEnvironment env,
    const HadopeMemoryBuffer mem_struct,
    int *dataset
);

void getIntArrayFromDevice(
    const HadopeEnvironment env,
    const HadopeMemoryBuffer mem_struct,
    int *dataset
);

void* getPinnedArrayFromDevice(
    const HadopeEnvironment* env,
    const HadopeMemoryBuffer* mem_struct,
    const size_t unit_size
);

void runTaskOnDataset(
    const HadopeEnvironment* env,
    const HadopeMemoryBuffer* mem_struct,
    const HadopeTask* task
);

void computePresenceArrayForDataset(
    const HadopeEnvironment* env,
    const HadopeMemoryBuffer* mem_struct,
    const HadopeTask* task,
  HadopeMemoryBuffer* presence
);

void exclusivePrefixSum(
    const HadopeEnvironment* env,
    const HadopeMemoryBuffer* presence,
    char* source,
    HadopeMemoryBuffer* result
);

void integerBitonicSort(
    const HadopeEnvironment* env,
    HadopeMemoryBuffer* input_dataset,
    HadopeTask* task
);

int sumIntegerDataset(
    const HadopeEnvironment* env,
    HadopeMemoryBuffer* input_dataset,
    char* source
);

void braidBuffers(
    const HadopeEnvironment* env,
    const HadopeTask* task,
    HadopeMemoryBuffer* fsts,
    HadopeMemoryBuffer* snds
);

int filteredBufferLength(
    const HadopeEnvironment* env,
    HadopeMemoryBuffer* presence,
    HadopeMemoryBuffer* input_dataset
);

void filterByScatteredWrites(
    const HadopeEnvironment* env,
    HadopeMemoryBuffer* input_dataset,
    HadopeMemoryBuffer* presence,
    HadopeMemoryBuffer* index_scan
);

void releaseTemporaryFilterBuffers(
    HadopeMemoryBuffer* presence,
    HadopeMemoryBuffer* index_scan
);

void releaseDeviceDataset(
    HadopeMemoryBuffer* dataset
);
#endif
