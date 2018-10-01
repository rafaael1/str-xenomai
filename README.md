# str-xenomai
Práticas com Xenomai para disciplina de Sistemas de Tempo Real

O Xenomai é um RTOS disponível para plataformas baseadas em linux, por isso o Xenomai pode ser rodado tanto em maquinas rodando alguma variação do linux como sistemas embardados como o Raspberry Pi que possibilita a programação de threads em tempo real, dentre outras características como o comprimento de deadlines e assim por diante…

Ao ser instalado, ele roda em conjunto com o kernel do Linux e, portanto, todas as API’s e drivers do Linux vão continuar sendo possíveis de se utilizar. O interessante é que seus programas legados continuam compatíveis mesmo após a instalação do Xenomai. Contudo, você precisa mudar algumas flags de compilação para substituir as suas chamadas POSIX do Linux para as POSIX do Xenomai. Fonte: [Projeto garagem](https://projetogaragem.wordpress.com/2015/11/22/emulando-xenomai-rtos/)

### Objetivos Gerais
Realização das práticas planejadas pelo curso _Design of Embedded Systems_ (DES) da Radboud University Nijmegen, em cima do framework Xenomai, para reforço de conceitos vistos na disciplina Sistemas de Tempo Real.

### Objetivos Específicos
- Desenvolver soluções para os exercícios do DES em cima do Xenomai;
- Executar as soluções em um ambiente simulado;
- Executar as soluções em um ambiente real;

## Usage:
- Virtual Box
- Raspberry Pi Model B  

### Exercicios resolvidos
1. Começando
2. Multitarefas
3. Semáforos
4. 

### Referências
Link: [Radboud University Nijmegen](http://www.cs.ru.nl/lab/xenomai/)
