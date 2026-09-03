#!/bin/bash

mkdir -p resultados

echo "instancia,execucao1,execucao2,execucao3,execucao4,execucao5,execucao6,execucao7,execucao8,execucao9,execucao10,custo_medio" > resultados/resultados.csv

instancias=(
    "a280"
    "ali535"
    "att48"
    "att532"
    "bayg29"
    "bays29"
    "berlin52"
    "bier127"
    "brazil58"
    "brg180"
    "burma14"
    "ch130"
    "ch150"
    "d198"
    "d493"
    "dantzig42"
    "eil101"
    "eil51"
    "eil76"
    "fl417"
    "fri26"
    "gil262"
    "gr120"
    "gr137"
    "gr17"
    "gr202"
    "gr21"
    "gr229"
    "gr24"
    "gr431"
    "gr48"
    "gr96"
    "hk48"
    "kroA100"
    "kroA150"
    "kroA200"
    "kroB100"
    "kroB150"
    "kroB200"
    "kroC100"
    "kroD100"
    "kroE100"
    "lin105"
    "lin318"
    "linhp318"
    "pcb442"
    "pr107"
    "pr124"
    "pr136"
    "pr144"
    "pr152"
    "pr226"
    "pr264"
    "pr299"
    "pr76"
    "rat195"
    "rat99"
    "rd100"
    "rd400"
    "si175"
    "si535"
    "st70"
    "swiss42"
    "ts225"
    "tsp225"
    "u159"
    "ulysses16"
    "ulysses22"
)

for nome in "${instancias[@]}"
do
    instancia="instances/$nome.tsp"

    soma=0
    custos=()

    echo "Processando $nome..."

    for i in {1..10}
    do
        saida=$(./tsp "$instancia")
        custo=$(echo "$saida" | tail -n 1)

        custos+=("$custo")
        soma=$(echo "$soma + $custo" | bc)
    done

    media=$(echo "scale=6; $soma / 10" | bc)

    echo -n "$nome" >> resultados/resultados.csv

    for custo in "${custos[@]}"
    do
        echo -n ",$custo" >> resultados/resultados.csv
    done

    echo ",$media" >> resultados/resultados.csv

    echo "  Custo médio: $media"
done

echo ""
echo "Experimentos concluídos!"
echo "Resultado salvo em resultados/resultados.csv"