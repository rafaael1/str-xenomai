## Xenomai

O Xenomai _Real-Time Framework for Linux_ (XENOMAI, 2007) é um framewok de tempo-real que trabalha em cooperação 
com o kernel do linux visando a um amplo suporte hard real-time a aplicações em espaço de usuário. Assim, o 
Xenomai adiciona uma camada de abstraçãoao sistema Linux, o qual adiciona comandos para lançamento e execução 
de tarefas em tempo real (Xenomai tasks), de modo a garantir que o linux não exerça nenhuma interferência nas 
tarefas de tempo real. Para tanto, todo o SO linux é lançado como uma tarefa de baixa prioridade por um 
micro-kernel de tempo real, o que garante que todas as tarefas de tempo real, lançadas pelo micro-kernel, nunca 
são interrompidas pelo Linux. De fato, o Linux só é executado quando nenhuma tarefa de tempo-real, com alta 
prioridade, está pronta para executar. O Xenomai depende do nanokernel de tempo real ADEOS para gerir interrupções 
em tempo real (_Ipipe: interrupt pipeline_).

O Xenomai visa primeiramente auxiliar desenvolvedores, que trabalham em um ambiente de tempo-real tradicional, migrarem 
para um ambiente GNU/Linux da maneira menos impactante possível, sem a necessidade de reescrever o código inteiramente, 
ou seja, real-time API para qualquer linux (GERUM, 2004).


### Referências
Link: [SISTEMA DE CONTROLE TEMPO REAL EMBARCADO PARA AUTOMAÇÃO DE MANOBRA DE ESTACIONAMENTO](https://core.ac.uk/download/pdf/30374185.pdf)
