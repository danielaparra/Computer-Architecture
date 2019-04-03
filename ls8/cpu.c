#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char** argv)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line[1024];

  if (argc != 2) {
    printf("usage: fileio filename\n");
    return;
  }

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    printf("Error opening file %s\n", argv[1]);
  }
  
  int address = 0;
  // printf("here1\n");
  while (fgets(line, 1024, fp) != NULL) {
  // printf("here2\n");
    if (line[0] == '#') {
      // printf("here skip\n");
      continue;
    }
    // printf("here3\n");
    char *binary_string = strndup(line, 8);
    // printf("%s", *binary_string);
    // printf("here4\n");
    unsigned char binary_val = strtol(binary_string, NULL, 2);
    // printf("here5\n");
    cpu->ram[address] = binary_val;
    // cpu_ram_write(cpu, address, binary_val);
    // printf("%d\n", cpu->ram[address]);
    free(binary_string);
    address++;
  }

  // printf("this\n");
  fclose(fp);
  
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Helper Functions
 */

/**
 * Read and return ram at address
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

/**
 * Write value to ram at address
 */
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}

/**
 * Figure out how many operands this next instruction requires
 */

int num_operands_needed(unsigned char IR) {
  switch (IR) {
    case LDI:
      return 2;

    case MUL:
      return 2;

    case PRN: 
      return 1;

    default:
      return 0;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    
    // 2. Figure out how many operands this next instruction requires
    int num_of_operands = num_operands_needed(IR); 

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA, operandB;

    switch (num_of_operands) {
      case 1:
        operandA = cpu_ram_read(cpu, cpu->PC + 1);
      case 2:
        operandA = cpu_ram_read(cpu, cpu->PC + 1);
        operandB = cpu_ram_read(cpu, cpu->PC + 2);
      default:
        break;
    }

    // 4. switch() over it to decide on a course of action.
    switch (IR) {
      // 5. Do whatever the instruction should do according to the spec.
      // 6. Move the PC to the next instruction.
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;
      case MUL:
        cpu->registers[operandA] *= cpu->registers[operandB];
        // printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 3;
        break;
      case HLT:
        running = 0;
        cpu->PC++;
        break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->ram, 0, 256);
  memset(cpu->registers, 0, 8);
  cpu->registers[R7] = SP; 
}