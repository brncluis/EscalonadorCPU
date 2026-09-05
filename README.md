# Scheduler

Simulador de escalonamento de tarefas críticas de voo em C, comparando rate-monotonic e earliest-deadline-first.
Disciplina Sistemas Operacionais (CESAR School).
Desenvolvido por Luis Alves

## Arquivos utilizados

- `scheduler.c` — código-fonte completo: leitura e validação do arquivo de entrada, motor de simulação (chegadas, deadlines, escolha de prioridade entre rate-monotonic e EDF), geração do histórico de execução, escrita do arquivo `.out` e o loop principal (`main`).
- `Makefile` — regra de compilação do executável `scheduler` e limpeza dos arquivos gerados.

## Sistema operacional

Desenvolvido e testado em **Ubuntu Linux**, arquitetura `x86_64`.

## Como compilar

```
make
```

Isso gera o executável `scheduler` a partir de `scheduler.c`.

Para recompilar do zero:

```
make clean
make
```

## Como executar

```
./scheduler <rate|edf> <arquivo>
```

Exemplo:

```
./scheduler rate aviao.txt
./scheduler edf aviao.txt
```

O arquivo de entrada contém o tempo total de simulação na primeira linha, seguido de uma tarefa por linha no formato `NOME PERIODO DEADLINE BURST`. Cada execução gera um arquivo de saída `rate_lhass.out` ou `edf_lhass.out`, dependendo do algoritmo escolhido, com o traço de execução e os totais de tarefas concluídas, com deadline perdido e mortas ao final da simulação.

## Como testar

Compare a saída do programa contra o exemplo do enunciado:

```
./scheduler rate aviao.txt
cat rate_lhass.out
```

Também é possível testar os dois algoritmos com o mesmo arquivo de entrada e comparar o comportamento entre eles:

```
./scheduler rate aviao.txt
./scheduler edf aviao.txt
diff rate_lhass.out edf_lhass.out
```

Casos de entrada inválida (número de argumentos incorreto, algoritmo diferente de `rate`/`edf`, arquivo inexistente, arquivo malformado, ou tarefa que viola `burst ≤ deadline ≤ periodo`) devem ser rejeitados com mensagem de erro em stderr, código de saída diferente de zero, e sem gerar nenhum arquivo `.out`.