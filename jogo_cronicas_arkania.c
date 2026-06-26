/*
 * ============================================================================
 *  CRONICAS DE ARKANIA - Livro Jogo RPG
 *  Baseado na serie "Caverna do Dragao"
 *  Disciplina: Programacao Estruturada - 2026.1
 *
 *  EQUIPE:
 *    - Artur de Paula
 *    - Matheus Linhares
 *    - Daniel Alvez
 *    
 *
 *  COMPILACAO: gcc jogo_cronicas_arkania.c -o jogo.exe
 *  EXECUCAO:   jogo.exe
 *

 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_NOME 50
#define MAX_DESC 500
#define MAX_OPCAO 100
#define NUM_PAGINAS 30
#define VIDA_INICIAL 100
#define ARQUIVO_SAVE "save_arkania.dat"

typedef struct {
    char nome[MAX_NOME];
    int moral;
    int coragem;
    int vida;
    int vidaMax;
    int paginaAtual;
} Jogador;

typedef struct {
    char descricao[MAX_DESC];
    char opcaoTexto[4][MAX_OPCAO];
    int opcaoDestino[4];
    int opcaoMoral[4];
    int opcaoCoragem[4];
    int temBatalha;
} Pagina;

Jogador jogador;
Pagina paginas[NUM_PAGINAS];

void inicializarPaginas();
void limparTela();
void limparBuffer();
void pausar();
void menuPrincipal();
void novoJogo();
void continuarJogo();
void creditos();
void tutorial();
void criarPersonagem();
void jogar();
void exibirStatus();
void combate(int dificuldade);
void salvarJogo();
int carregarJogo();
void finalJogo(int tipo);

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}
void criarPersonagem() {
    limparTela();
    printf("========================================\n");
    printf("     CRIACAO DO PERSONAGEM              \n");
    printf("========================================\n\n");
    printf("Digite o nome do seu heroi: ");
    scanf(" %49[^\n]", jogador.nome);
    limparBuffer();
    jogador.moral = 0;
    jogador.coragem = 0;
    jogador.vida = VIDA_INICIAL;
    jogador.vidaMax = VIDA_INICIAL;
    jogador.paginaAtual = 0;
    printf("\nSeja bem-vindo, %s!\n", jogador.nome);
    printf("Suas escolhas ao longo da jornada determinarao seu destino.\n");
    pausar();
}

void exibirStatus() {
    printf("\n--- %s ---\n", jogador.nome);
    printf("Vida: %d/%d\n", jogador.vida, jogador.vidaMax);
    printf("Moral: %d | Coragem: %d\n", jogador.moral, jogador.coragem);
}

void salvarJogo() {
    FILE *arq = fopen(ARQUIVO_SAVE, "wb");
    if (arq == NULL) { printf("Erro ao salvar!\n"); return; }
    fwrite(&jogador, sizeof(Jogador), 1, arq);
    fclose(arq);
}

int carregarJogo() {
    FILE *arq = fopen(ARQUIVO_SAVE, "rb");
    if (arq == NULL) return 0;
    fread(&jogador, sizeof(Jogador), 1, arq);
    fclose(arq);
    return 1;
}
void combate(int dificuldade) {
    int inimigoVida = 30 + (dificuldade * 10);
    int inimigoAtaque = 5 + (dificuldade * 3);
    int inimigoDefesa = 2 + dificuldade;
    int turno = 0, escolha, dadoJogador, dadoInimigo, dano;
    char nomeInimigo[MAX_NOME];
    if (dificuldade >= 4) strcpy(nomeInimigo, "Vorak");
    else if (dificuldade == 3) strcpy(nomeInimigo, "Dragao");
    else if (dificuldade == 2) strcpy(nomeInimigo, "Esqueleto");
    else if (dificuldade == 1) strcpy(nomeInimigo, "Orc");
    else strcpy(nomeInimigo, "Goblin");
    printf("\n=== COMBATE CONTRA %s ===\n", nomeInimigo);
    printf("Vida do inimigo: %d\n\n", inimigoVida);
    pausar();
    while (jogador.vida > 0 && inimigoVida > 0) {
        turno++;
        limparTela();
        printf("=== Turno %d ===\n", turno);
        printf("Voce: %d/%d de vida\n", jogador.vida, jogador.vidaMax);
        printf("%s: %d de vida\n\n", nomeInimigo, inimigoVida);
        printf("1. Atacar\n2. Defender\n3. Fugir\n4. Ataque especial\nEscolha: ");
        scanf("%d", &escolha);
        limparBuffer();
        dadoJogador = (rand() % 20) + 1;
        dadoInimigo = (rand() % 20) + 1;
        dano = 0;
        if (escolha == 1) {
            dano = dadoJogador + 5 - inimigoDefesa;
            if (dano < 0) dano = 0;
            inimigoVida -= dano;
            printf("\nVoce ataca e causa %d de dano!\n", dano);
        } else if (escolha == 2) {
            printf("\nVoce se prepara para defender!\n");
        } else if (escolha == 3) {
            if (dadoJogador > 10) { printf("\nFugiu!\n"); pausar(); return; }
            else printf("\nFalhou ao fugir!\n");
        } else if (escolha == 4) {
            dano = (dadoJogador + 5 - inimigoDefesa) * 2;
            if (dano < 0) dano = 0;
            inimigoVida -= dano;
            jogador.vida -= 8;
            printf("\nAtaque especial causa %d de dano!\n", dano);
            printf("Mas perde 8 de vida.\n");
        } else { printf("Invalido!\n"); }
        if (inimigoVida <= 0) { printf("\n%s derrotado!\n", nomeInimigo); pausar(); return; }
        if (escolha == 2) dano = (dadoInimigo + inimigoAtaque - 5) / 2;
        else dano = dadoInimigo + inimigoAtaque - 5;
        if (dano < 0) dano = 0;
        jogador.vida -= dano;
        printf("%s causa %d de dano!\n", nomeInimigo, dano);
        if (jogador.vida <= 0) { jogador.vida = 0; printf("\nMorreu!\n"); pausar(); return; }
        pausar();
    }
}

void finalJogo(int tipo) {
    limparTela();
    printf("========================================\n");
    printf("              FIM                        \n");
    printf("========================================\n\n");
    if (tipo == 1) {
        printf("  FINAL BOM!\n\n");
        printf("  Sua bondade e coragem inspiraram Arkânia.\n");
        printf("  Vorak foi derrotado e o reino esta livre!\n");
        printf("  Os seis amigos sao celebrados como herois.\n");
        printf("  Mestre Arkhanon envia todos de volta para casa.\n");
        printf("  A verdadeira forca veio da amizade.\n\n");
    } else if (tipo == 2) {
        printf("  FINAL MEDIO!\n\n");
        printf("  Vorak caiu, mas o custo foi alto.\n");
        printf("  Nem todos sobreviveram a jornada.\n");
        printf("  Arkânia esta salva, mas ha cicatrizes.\n");
        printf("  Talvez com mais compaixao ou coragem\n");
        printf("  o final pudesse ser diferente...\n\n");
    } else {
        printf("  FINAL RUIM!\n\n");
        printf("  As trevas consumiram Arkânia.\n");
        printf("  Vorak governa com punho de ferro.\n");
        printf("  Seus amigos estao perdidos para sempre.\n");
        printf("  A esperanca se apagou neste reino.\n\n");
    }
    printf("  Moral: %d | Coragem: %d\n", jogador.moral, jogador.coragem);
    printf("\n1. Voltar ao Menu\n2. Sair\nEscolha: ");
    { int op; scanf("%d", &op); limparBuffer(); if (op == 1) menuPrincipal(); else exit(0); }
}
void jogar() {
    int pagAtual = jogador.paginaAtual;
    int escolha, continuar = 1, i;
    while (continuar == 1) {
        if (pagAtual < 0 || pagAtual >= NUM_PAGINAS) break;
        limparTela();
        printf("=== Pagina %d de %d ===\n\n", pagAtual + 1, NUM_PAGINAS);
        printf("%s\n\n", paginas[pagAtual].descricao);
        if (paginas[pagAtual].temBatalha == 1) {
            combate(pagAtual / 5);
            if (jogador.vida <= 0) {
                printf("\nSua jornada termina aqui...\n");
                pausar();
                finalJogo(3);
                return;
            }
        }
        for (i = 0; i < 4; i++)
            printf("%d. %s\n", i + 1, paginas[pagAtual].opcaoTexto[i]);
        printf("5. Salvar e Sair\n\nEscolha: ");
        scanf("%d", &escolha);
        limparBuffer();
        if (escolha >= 1 && escolha <= 4) {
            int idx = escolha - 1;
            jogador.moral += paginas[pagAtual].opcaoMoral[idx];
            jogador.coragem += paginas[pagAtual].opcaoCoragem[idx];
            if (jogador.moral > 10) jogador.moral = 10;
            if (jogador.moral < -10) jogador.moral = -10;
            if (jogador.coragem > 10) jogador.coragem = 10;
            if (jogador.coragem < -10) jogador.coragem = -10;
            jogador.paginaAtual = paginas[pagAtual].opcaoDestino[idx];
            pagAtual = jogador.paginaAtual;
            salvarJogo();
        } else if (escolha == 5) {
            jogador.paginaAtual = pagAtual;
            salvarJogo();
            printf("Jogo salvo!\n");
            pausar();
            menuPrincipal();
            return;
        } else { printf("Opcao invalida!\n"); pausar(); }
        if (pagAtual >= 27 && pagAtual <= 29) {
            int soma = jogador.moral + jogador.coragem;
            if (soma > 6) finalJogo(1);
            else if (soma < -6) finalJogo(3);
            else finalJogo(2);
            return;
        }
    }
}

void menuPrincipal() {
    int opcao;
    while (1) {
        limparTela();
        printf("========================================\n");
        printf("   CRONICAS DE ARKANIA                  \n");
        printf("   A Jornada dos Herois                 \n");
        printf("========================================\n\n");
        printf("1. Novo Jogo\n2. Continuar\n3. Creditos\n");
        printf("4. Tutorial\n5. Sair\n\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();
        switch (opcao) {
            case 1: novoJogo(); break;
            case 2: continuarJogo(); break;
            case 3: creditos(); break;
            case 4: tutorial(); break;
            case 5: printf("Ate logo!\n"); exit(0);
            default: printf("Opcao invalida!\n"); pausar();
        }
    }
}

void novoJogo() {
    criarPersonagem();
    jogador.paginaAtual = 0;
    salvarJogo();
    jogar();
}

void continuarJogo() {
    if (carregarJogo() == 1) {
        printf("Bem-vindo de volta, %s! Pagina %d.\n", jogador.nome, jogador.paginaAtual + 1);
        pausar();
        jogar();
    } else { printf("Nenhum save encontrado!\n"); pausar(); }
}

void creditos() {
    limparTela();
    printf("========================================\n");
    printf("   CREDITOS                             \n");
    printf("========================================\n\n");
    printf("CRONICAS DE ARKANIA: A JORNADA DOS HEROIS\n");
    printf("Baseado na serie Caverna do Dragao\n\n");
    printf("DESENVOLVEDORES:\n- Artur de Paula\n- [Colega 1]\n- [Colega 2]\n- [Colega 3]\n\n");
    printf("Programacao Estruturada - 2026.1\n\n");
    pausar();
}

void tutorial() {
    limparTela();
    printf("========================================\n");
    printf("   TUTORIAL                             \n");
    printf("========================================\n\n");
    printf("CRONICAS DE ARKANIA e um livro-jogo.\n\n");
    printf("- Cada pagina apresenta uma cena da historia\n");
    printf("- Escolha entre 4 opcoes numeradas\n");
    printf("- Suas escolhas afetam MORAL e CORAGEM\n");
    printf("- O final depende da soma desses valores\n\n");
    printf("FINAL BOM:   moral + coragem > 6\n");
    printf("FINAL MEDIO: -6 a 6\n");
    printf("FINAL RUIM:  moral + coragem < -6\n\n");
    pausar();
}
void inicializarPaginas() {
    int i, j;
    for (i = 0; i < NUM_PAGINAS; i++) {
        paginas[i].descricao[0] = '\0';
        paginas[i].temBatalha = 0;
        for (j = 0; j < 4; j++) {
            paginas[i].opcaoTexto[j][0] = '\0';
            paginas[i].opcaoDestino[j] = 0;
            paginas[i].opcaoMoral[j] = 0;
            paginas[i].opcaoCoragem[j] = 0;
        }
    }
    strcpy(paginas[0].descricao,
        "Uma tarde no parque de diversoes. Voce e seus cinco amigos -\n"
        "Kael, Erik, Lyra, Naya, Finn e Torben - andam na montanha-russa.\n"
        "De repente, um portal magico se abre no ceu e todos sao sugados.\n"
        "Voce acorda em uma clareira em Arkânia. Mestre Arkhanon aparece\n"
        "e diz que Vorak, o senhor das trevas, ameaca o reino. Reaja:");
    strcpy(paginas[0].opcaoTexto[0], "Aceitar a missao com coragem");
    strcpy(paginas[0].opcaoTexto[1], "Pedir mais explicacoes");
    strcpy(paginas[0].opcaoTexto[2], "Tentar voltar para casa");
    strcpy(paginas[0].opcaoTexto[3], "Seguir sozinho pela floresta");
    paginas[0].opcaoDestino[0] = 1; paginas[0].opcaoDestino[1] = 2;
    paginas[0].opcaoDestino[2] = 3; paginas[0].opcaoDestino[3] = 4;
    paginas[0].opcaoMoral[0] = 1; paginas[0].opcaoMoral[1] = 1;
    paginas[0].opcaoMoral[2] = 0; paginas[0].opcaoMoral[3] = -2;
    paginas[0].opcaoCoragem[0] = 2; paginas[0].opcaoCoragem[1] = 0;
    paginas[0].opcaoCoragem[2] = -1; paginas[0].opcaoCoragem[3] = 1;
    strcpy(paginas[1].descricao,
        "Arkhanon sorri e lhe da uma espada. Seus amigos se reunicm.\n"
        "O grupo segue pela Floresta das Sombras. Apos horas de caminhada,\n"
        "encontram um elfo ferido caido a beira do caminho. Ele geme de dor.\n"
        "O que fazer com o elfo?");
    strcpy(paginas[1].opcaoTexto[0], "Ajudar o elfo sem hesitar");
    strcpy(paginas[1].opcaoTexto[1], "Pedir algo em troca");
    strcpy(paginas[1].opcaoTexto[2], "Ignorar e seguir em frente");
    strcpy(paginas[1].opcaoTexto[3], "Usar o elfo como isca");
    paginas[1].opcaoDestino[0] = 5; paginas[1].opcaoDestino[1] = 5;
    paginas[1].opcaoDestino[2] = 5; paginas[1].opcaoDestino[3] = 5;
    paginas[1].opcaoMoral[0] = 2; paginas[1].opcaoMoral[1] = 0;
    paginas[1].opcaoMoral[2] = -2; paginas[1].opcaoMoral[3] = -3;
    paginas[1].opcaoCoragem[0] = 1; paginas[1].opcaoCoragem[1] = -1;
    paginas[1].opcaoCoragem[2] = -1; paginas[1].opcaoCoragem[3] = 1;

    strcpy(paginas[2].descricao,
        "Arkhanon explica que Vorak roubou o Cristal da Luz e corrompe\n"
        "a terra. Sem ele, o portal nao se abre. Precisam derrotar Vorak.\n"
        "O grupo entra na floresta. Encontram um elfo ferido no chao.\n"
        "O elfo pede ajuda. Sua decisao:");
    strcpy(paginas[2].opcaoTexto[0], "Curar o elfo com seus recursos");
    strcpy(paginas[2].opcaoTexto[1], "Perguntar o que houve antes");
    strcpy(paginas[2].opcaoTexto[2], "Ignorar e seguir caminho");
    strcpy(paginas[2].opcaoTexto[3], "Atacar pensando ser armadilha");
    paginas[2].opcaoDestino[0] = 5; paginas[2].opcaoDestino[1] = 5;
    paginas[2].opcaoDestino[2] = 5; paginas[2].opcaoDestino[3] = 6;
    paginas[2].opcaoMoral[0] = 2; paginas[2].opcaoMoral[1] = 1;
    paginas[2].opcaoMoral[2] = -1; paginas[2].opcaoMoral[3] = -3;
    paginas[2].opcaoCoragem[0] = 1; paginas[2].opcaoCoragem[1] = 0;
    paginas[2].opcaoCoragem[2] = -2; paginas[2].opcaoCoragem[3] = 1;

    strcpy(paginas[3].descricao,
        "Arkhanon diz que nao ha volta ate derrotar Vorak. Seus amigos\n"
        "olham para voce. Relutante, aceita liderar o grupo pela floresta.\n"
        "No caminho, um elfo ferido pede socorro. Qual sua atitude?");
    strcpy(paginas[3].opcaoTexto[0], "Ajudar sem pensar duas vezes");
    strcpy(paginas[3].opcaoTexto[1], "Ajudar mas reclamar");
    strcpy(paginas[3].opcaoTexto[2], "Passar direto");
    strcpy(paginas[3].opcaoTexto[3], "Mandam os outros ajudarem");
    paginas[3].opcaoDestino[0] = 5; paginas[3].opcaoDestino[1] = 5;
    paginas[3].opcaoDestino[2] = 5; paginas[3].opcaoDestino[3] = 5;
    paginas[3].opcaoMoral[0] = 2; paginas[3].opcaoMoral[1] = 0;
    paginas[3].opcaoMoral[2] = -2; paginas[3].opcaoMoral[3] = -1;
    paginas[3].opcaoCoragem[0] = 1; paginas[3].opcaoCoragem[1] = -1;
    paginas[3].opcaoCoragem[2] = -1; paginas[3].opcaoCoragem[3] = 0;

    strcpy(paginas[4].descricao,
        "Voce segue sozinho pela floresta. Se perde. Seus amigos vao\n"
        "atras de voce. Kael diz que nao vao te abandonar. O grupo se\n"
        "reune. Encontram um elfo ferido no chao. Como age?");
    strcpy(paginas[4].opcaoTexto[0], "Ajudar o elfo e agradecer aos amigos");
    strcpy(paginas[4].opcaoTexto[1], "Ignorar o elfo, focar em sair");
    strcpy(paginas[4].opcaoTexto[2], "Interrogar o elfo");
    strcpy(paginas[4].opcaoTexto[3], "Culpam os amigos pelos problemas");
    paginas[4].opcaoDestino[0] = 5; paginas[4].opcaoDestino[1] = 5;
    paginas[4].opcaoDestino[2] = 6; paginas[4].opcaoDestino[3] = 6;
    paginas[4].opcaoMoral[0] = 1; paginas[4].opcaoMoral[1] = -2;
    paginas[4].opcaoMoral[2] = -1; paginas[4].opcaoMoral[3] = -2;
    paginas[4].opcaoCoragem[0] = 1; paginas[4].opcaoCoragem[1] = 0;
    paginas[4].opcaoCoragem[2] = 0; paginas[4].opcaoCoragem[3] = -1;
    strcpy(paginas[5].descricao,
        "O elfo Lian agradece pela ajuda. Ele explica que Vorak enviou\n"
        "goblins para atacar a Vila dos Elfos da Lua. Ele da um amuleto.\n"
        "De repente, GOBLINS SALTAM DAS MOITAS! E uma emboscada!");
    strcpy(paginas[5].opcaoTexto[0], "Enfrentar os goblins de frente");
    strcpy(paginas[5].opcaoTexto[1], "Usar armadilhas do ambiente");
    strcpy(paginas[5].opcaoTexto[2], "Proteger o elfo primeiro");
    strcpy(paginas[5].opcaoTexto[3], "Tentar negociar com os goblins");
    paginas[5].opcaoDestino[0] = 6; paginas[5].opcaoDestino[1] = 6;
    paginas[5].opcaoDestino[2] = 6; paginas[5].opcaoDestino[3] = 6;
    paginas[5].opcaoMoral[0] = 0; paginas[5].opcaoMoral[1] = 1;
    paginas[5].opcaoMoral[2] = 2; paginas[5].opcaoMoral[3] = -1;
    paginas[5].opcaoCoragem[0] = 2; paginas[5].opcaoCoragem[1] = 1;
    paginas[5].opcaoCoragem[2] = 0; paginas[5].opcaoCoragem[3] = -1;
    paginas[5].temBatalha = 1;

    strcpy(paginas[6].descricao,
        "Vitoria! Os goblins fugiram. Lian agradece e mostra o caminho.\n"
        "No chao, um mapa com duas rotas para o Castelo de Vorak:\n"
        "desfiladeiro (curto perigoso) ou masmorra (longo seguro).");
    strcpy(paginas[6].opcaoTexto[0], "Ir pelo desfiladeiro arriscado");
    strcpy(paginas[6].opcaoTexto[1], "Ir pela masmorra segura");
    strcpy(paginas[6].opcaoTexto[2], "Ir para a vila dos elfos");
    strcpy(paginas[6].opcaoTexto[3], "Seguir rastro dos goblins");
    paginas[6].opcaoDestino[0] = 8; paginas[6].opcaoDestino[1] = 13;
    paginas[6].opcaoDestino[2] = 7; paginas[6].opcaoDestino[3] = 11;
    paginas[6].opcaoMoral[0] = -1; paginas[6].opcaoMoral[1] = 0;
    paginas[6].opcaoMoral[2] = 2; paginas[6].opcaoMoral[3] = -2;
    paginas[6].opcaoCoragem[0] = 2; paginas[6].opcaoCoragem[1] = 0;
    paginas[6].opcaoCoragem[2] = 0; paginas[6].opcaoCoragem[3] = 1;

    strcpy(paginas[7].descricao,
        "Vila dos Elfos da Lua. Lampadas de cristal flutuam, musica ecoa.\n"
        "Rainha Selene oferece abrigo. Finn descobre pergaminho sobre\n"
        "o ponto fraco de Vorak: o cristal no cetro dele.");
    strcpy(paginas[7].opcaoTexto[0], "Treinar com os elfos");
    strcpy(paginas[7].opcaoTexto[1], "Descansar e recuperar forcas");
    strcpy(paginas[7].opcaoTexto[2], "Estudar o pergaminho com Finn");
    strcpy(paginas[7].opcaoTexto[3], "Explorar segredos da vila");
    paginas[7].opcaoDestino[0] = 10; paginas[7].opcaoDestino[1] = 9;
    paginas[7].opcaoDestino[2] = 10; paginas[7].opcaoDestino[3] = 9;
    paginas[7].opcaoMoral[0] = 0; paginas[7].opcaoMoral[1] = 1;
    paginas[7].opcaoMoral[2] = 1; paginas[7].opcaoMoral[3] = 0;
    paginas[7].opcaoCoragem[0] = 2; paginas[7].opcaoCoragem[1] = -1;
    paginas[7].opcaoCoragem[2] = 0; paginas[7].opcaoCoragem[3] = 1;

    strcpy(paginas[8].descricao,
        "Desfiladeiro perigoso. Pedras caem, vento uiva. Ponte de corda\n"
        "atravessa o abismo. Urubus gigantes atacam o grupo!");
    strcpy(paginas[8].opcaoTexto[0], "Lutar contra os urubus");
    strcpy(paginas[8].opcaoTexto[1], "Correr pela ponte");
    strcpy(paginas[8].opcaoTexto[2], "Usar tocha para espanta-los");
    strcpy(paginas[8].opcaoTexto[3], "Se esconder e esperar");
    paginas[8].opcaoDestino[0] = 10; paginas[8].opcaoDestino[1] = 11;
    paginas[8].opcaoDestino[2] = 10; paginas[8].opcaoDestino[3] = 9;
    paginas[8].opcaoMoral[0] = 1; paginas[8].opcaoMoral[1] = -1;
    paginas[8].opcaoMoral[2] = 1; paginas[8].opcaoMoral[3] = 0;
    paginas[8].opcaoCoragem[0] = 2; paginas[8].opcaoCoragem[1] = 1;
    paginas[8].opcaoCoragem[2] = 0; paginas[8].opcaoCoragem[3] = -2;
    paginas[8].temBatalha = 1;

    strcpy(paginas[9].descricao,
        "Noite de descanso restaura as forcas. Rainha Selene da pocoes.\n"
        "O grupo segue para o castelo. A floresta da lugar a montanhas.\n"
        "Avistam o Castelo de Vorak, imponente e sombrio ao longe.");
    strcpy(paginas[9].opcaoTexto[0], "Avancar direto para o castelo");
    strcpy(paginas[9].opcaoTexto[1], "Reconhecer a area primeiro");
    strcpy(paginas[9].opcaoTexto[2], "Acampar e planejar");
    strcpy(paginas[9].opcaoTexto[3], "Procurar entrada secreta");
    paginas[9].opcaoDestino[0] = 12; paginas[9].opcaoDestino[1] = 12;
    paginas[9].opcaoDestino[2] = 12; paginas[9].opcaoDestino[3] = 12;
    paginas[9].opcaoMoral[0] = 0; paginas[9].opcaoMoral[1] = 1;
    paginas[9].opcaoMoral[2] = 0; paginas[9].opcaoMoral[3] = 0;
    paginas[9].opcaoCoragem[0] = 2; paginas[9].opcaoCoragem[1] = 0;
    paginas[9].opcaoCoragem[2] = -1; paginas[9].opcaoCoragem[3] = 1;
    strcpy(paginas[10].descricao,
        "Preparados, seguem para o castelo. Muros de pedra negra, torres\n"
        "pontiagudas. Um dragao vermelho circula sobre a fortaleza.\n"
        "Gargulas observam de cima. Como entrar?");
    strcpy(paginas[10].opcaoTexto[0], "Portao principal disfarcado");
    strcpy(paginas[10].opcaoTexto[1], "Entrar pelo esgoto");
    strcpy(paginas[10].opcaoTexto[2], "Escalar a muralha");
    strcpy(paginas[10].opcaoTexto[3], "Tomar armaduras de patrulha");
    paginas[10].opcaoDestino[0] = 12; paginas[10].opcaoDestino[1] = 12;
    paginas[10].opcaoDestino[2] = 13; paginas[10].opcaoDestino[3] = 11;
    paginas[10].opcaoMoral[0] = 0; paginas[10].opcaoMoral[1] = -1;
    paginas[10].opcaoMoral[2] = 0; paginas[10].opcaoMoral[3] = -1;
    paginas[10].opcaoCoragem[0] = 1; paginas[10].opcaoCoragem[1] = 0;
    paginas[10].opcaoCoragem[2] = 2; paginas[10].opcaoCoragem[3] = 0;

    strcpy(paginas[11].descricao,
        "Sua abordagem agressiva alerta os guardas do castelo. Sinos\n"
        "tocam. Guardas correm para todos os lados. Voces precisam\n"
        "se esconder rapidamente. Onde se esconder?");
    strcpy(paginas[11].opcaoTexto[0], "Se esconder nos arbustos");
    strcpy(paginas[11].opcaoTexto[1], "Correr para o bosque");
    strcpy(paginas[11].opcaoTexto[2], "Lutar contra os guardas");
    strcpy(paginas[11].opcaoTexto[3], "Se passar por servos");
    paginas[11].opcaoDestino[0] = 12; paginas[11].opcaoDestino[1] = 9;
    paginas[11].opcaoDestino[2] = 12; paginas[11].opcaoDestino[3] = 13;
    paginas[11].opcaoMoral[0] = 0; paginas[11].opcaoMoral[1] = -1;
    paginas[11].opcaoMoral[2] = -2; paginas[11].opcaoMoral[3] = 0;
    paginas[11].opcaoCoragem[0] = -1; paginas[11].opcaoCoragem[1] = -1;
    paginas[11].opcaoCoragem[2] = 2; paginas[11].opcaoCoragem[3] = 0;
    paginas[11].temBatalha = 1;

    strcpy(paginas[12].descricao,
        "Dentro do castelo. Corredores escuros, tochas nas paredes.\n"
        "Guardas fazem ronda. Ouvem gritos de prisioneiros vindos da\n"
        "masmorra. O que fazer?");
    strcpy(paginas[12].opcaoTexto[0], "Libertar os prisioneiros");
    strcpy(paginas[12].opcaoTexto[1], "Ir direto ao sala do trono");
    strcpy(paginas[12].opcaoTexto[2], "Explorar o castelo primeiro");
    strcpy(paginas[12].opcaoTexto[3], "Armar uma emboscada para Vorak");
    paginas[12].opcaoDestino[0] = 14; paginas[12].opcaoDestino[1] = 15;
    paginas[12].opcaoDestino[2] = 13; paginas[12].opcaoDestino[3] = 14;
    paginas[12].opcaoMoral[0] = 2; paginas[12].opcaoMoral[1] = -1;
    paginas[12].opcaoMoral[2] = 0; paginas[12].opcaoMoral[3] = -1;
    paginas[12].opcaoCoragem[0] = 1; paginas[12].opcaoCoragem[1] = 2;
    paginas[12].opcaoCoragem[2] = 0; paginas[12].opcaoCoragem[3] = 1;

    strcpy(paginas[13].descricao,
        "Explorando encontra uma sala de mapas. Ha plantas do castelo\n"
        "e uma passagem secreta para o sala do trono. Lyra encontra\n"
        "uma adaga encantada em um baú. Como proceder?");
    strcpy(paginas[13].opcaoTexto[0], "Usar a passagem secreta");
    strcpy(paginas[13].opcaoTexto[1], "Ir pela escadaria principal");
    strcpy(paginas[13].opcaoTexto[2], "Voltar e libertar prisioneiros");
    strcpy(paginas[13].opcaoTexto[3], "Procurar mais salas secretas");
    paginas[13].opcaoDestino[0] = 15; paginas[13].opcaoDestino[1] = 15;
    paginas[13].opcaoDestino[2] = 14; paginas[13].opcaoDestino[3] = 14;
    paginas[13].opcaoMoral[0] = 0; paginas[13].opcaoMoral[1] = 0;
    paginas[13].opcaoMoral[2] = 2; paginas[13].opcaoMoral[3] = 0;
    paginas[13].opcaoCoragem[0] = 1; paginas[13].opcaoCoragem[1] = 2;
    paginas[13].opcaoCoragem[2] = 0; paginas[13].opcaoCoragem[3] = 1;

    strcpy(paginas[14].descricao,
        "Prisioneiros libertos! Eles se juntam a sua causa. Entre eles\n"
        "esta um velho mago que conhece o ponto fraco de Vorak. Ele\n"
        "ensina um feitico para quebrar o cristal do cetro.");
    strcpy(paginas[14].opcaoTexto[0], "Aprender o feitico");
    strcpy(paginas[14].opcaoTexto[1], "Preferem forca bruta");
    strcpy(paginas[14].opcaoTexto[2], "Perguntar sobre tesouros");
    strcpy(paginas[14].opcaoTexto[3], "Levar os prisioneiros para fora");
    paginas[14].opcaoDestino[0] = 16; paginas[14].opcaoDestino[1] = 15;
    paginas[14].opcaoDestino[2] = 16; paginas[14].opcaoDestino[3] = 15;
    paginas[14].opcaoMoral[0] = 1; paginas[14].opcaoMoral[1] = -1;
    paginas[14].opcaoMoral[2] = -1; paginas[14].opcaoMoral[3] = 2;
    paginas[14].opcaoCoragem[0] = 0; paginas[14].opcaoCoragem[1] = 2;
    paginas[14].opcaoCoragem[2] = 0; paginas[14].opcaoCoragem[3] = -1;
    strcpy(paginas[15].descricao,
        "Salao do trono. Vorak esta sentado em seu trono de ossos.\n"
        "O cristal no cetro pulsa com energia negra. O dragao dorme\n"
        "a seus pes. Vorak ri: 'Entao vieram morrer, tolos!'");
    strcpy(paginas[15].opcaoTexto[0], "Atacar Vorak diretamente");
    strcpy(paginas[15].opcaoTexto[1], "Mirar no cristal do cetro");
    strcpy(paginas[15].opcaoTexto[2], "Usar o feitico dos prisioneiros");
    strcpy(paginas[15].opcaoTexto[3], "Provocar Vorak para errar");
    paginas[15].opcaoDestino[0] = 17; paginas[15].opcaoDestino[1] = 18;
    paginas[15].opcaoDestino[2] = 19; paginas[15].opcaoDestino[3] = 16;
    paginas[15].opcaoMoral[0] = 0; paginas[15].opcaoMoral[1] = 1;
    paginas[15].opcaoMoral[2] = 2; paginas[15].opcaoMoral[3] = -1;
    paginas[15].opcaoCoragem[0] = 2; paginas[15].opcaoCoragem[1] = 1;
    paginas[15].opcaoCoragem[2] = 0; paginas[15].opcaoCoragem[3] = 1;

    strcpy(paginas[16].descricao,
        "Vorak levanta e ataca com magia negra! O chao treme.\n"
        "O dragao acorda e ruge. O grupo precisa agir rapido!\n"
        "A batalha final comeca!");
    strcpy(paginas[16].opcaoTexto[0], "Enfrentar Vorak com espada");
    strcpy(paginas[16].opcaoTexto[1], "Tentar quebrar o cristal");
    strcpy(paginas[16].opcaoTexto[2], "Comandar os aliados");
    strcpy(paginas[16].opcaoTexto[3], "Usar magia dos elfos");
    paginas[16].opcaoDestino[0] = 20; paginas[16].opcaoDestino[1] = 18;
    paginas[16].opcaoDestino[2] = 21; paginas[16].opcaoDestino[3] = 19;
    paginas[16].opcaoMoral[0] = 0; paginas[16].opcaoMoral[1] = 1;
    paginas[16].opcaoMoral[2] = 1; paginas[16].opcaoMoral[3] = 0;
    paginas[16].opcaoCoragem[0] = 2; paginas[16].opcaoCoragem[1] = 1;
    paginas[16].opcaoCoragem[2] = 0; paginas[16].opcaoCoragem[3] = 0;
    paginas[16].temBatalha = 1;

    strcpy(paginas[17].descricao,
        "Ataque frontal! Voce corre contra Vorak. Ele ergue o cetro\n"
        "e uma barreira de energia aparece. Seus amigos atacam juntos.\n"
        "Kael atira flechas, Torben golpeia com o tacape.");
    strcpy(paginas[17].opcaoTexto[0], "Golpear a barreira com forca");
    strcpy(paginas[17].opcaoTexto[1], "Flanquear pela esquerda");
    strcpy(paginas[17].opcaoTexto[2], "Pedir ajuda de Finn");
    strcpy(paginas[17].opcaoTexto[3], "Esperar Vorak se cansar");
    paginas[17].opcaoDestino[0] = 20; paginas[17].opcaoDestino[1] = 18;
    paginas[17].opcaoDestino[2] = 19; paginas[17].opcaoDestino[3] = 15;
    paginas[17].opcaoMoral[0] = 0; paginas[17].opcaoMoral[1] = 0;
    paginas[17].opcaoMoral[2] = 1; paginas[17].opcaoMoral[3] = -1;
    paginas[17].opcaoCoragem[0] = 2; paginas[17].opcaoCoragem[1] = 1;
    paginas[17].opcaoCoragem[2] = 0; paginas[17].opcaoCoragem[3] = -2;

    strcpy(paginas[18].descricao,
        "Voce mira no cristal! Com precisao, lanca sua arma contra\n"
        "o cetro. O cristal trinca! Vorak grita de raiva e parte\n"
        "para cima de voce. O dragao cospe fogo!");
    strcpy(paginas[18].opcaoTexto[0], "Desviar e atacar de novo");
    strcpy(paginas[18].opcaoTexto[1], "Proteger o cristal trincado");
    strcpy(paginas[18].opcaoTexto[2], "Lutar contra o dragao");
    strcpy(paginas[18].opcaoTexto[3], "Recuar e reagrupar");
    paginas[18].opcaoDestino[0] = 19; paginas[18].opcaoDestino[1] = 20;
    paginas[18].opcaoDestino[2] = 21; paginas[18].opcaoDestino[3] = 17;
    paginas[18].opcaoMoral[0] = 0; paginas[18].opcaoMoral[1] = 1;
    paginas[18].opcaoMoral[2] = 0; paginas[18].opcaoMoral[3] = -1;
    paginas[18].opcaoCoragem[0] = 2; paginas[18].opcaoCoragem[1] = 0;
    paginas[18].opcaoCoragem[2] = 2; paginas[18].opcaoCoragem[3] = -1;
    paginas[18].temBatalha = 1;

    strcpy(paginas[19].descricao,
        "O feitico funciona! Uma luz dourada envolve o cristal.\n"
        "Vorak uiva de dor. O cristal comeca a se desfazer. Mas\n"
        "Vorak faz um ultimo ataque desesperado!");
    strcpy(paginas[19].opcaoTexto[0], "Finalizar Vorak");
    strcpy(paginas[19].opcaoTexto[1], "Proteger os amigos");
    strcpy(paginas[19].opcaoTexto[2], "Capturar Vorak vivo");
    strcpy(paginas[19].opcaoTexto[3], "Pegar o cetro quebrado");
    paginas[19].opcaoDestino[0] = 20; paginas[19].opcaoDestino[1] = 21;
    paginas[19].opcaoDestino[2] = 20; paginas[19].opcaoDestino[3] = 21;
    paginas[19].opcaoMoral[0] = 0; paginas[19].opcaoMoral[1] = 2;
    paginas[19].opcaoMoral[2] = 2; paginas[19].opcaoMoral[3] = -2;
    paginas[19].opcaoCoragem[0] = 2; paginas[19].opcaoCoragem[1] = 0;
    paginas[19].opcaoCoragem[2] = 0; paginas[19].opcaoCoragem[3] = 1;
    paginas[19].temBatalha = 1;
    strcpy(paginas[20].descricao,
        "Vorak cai de joelhos. O cristal se apaga. 'Impossivel...'\n"
        "Ele murmura enquanto suas vestes perdem a cor escura.\n"
        "O dragao, liberto do controle, voa para longe. Vitoria!");
    strcpy(paginas[20].opcaoTexto[0], "Comemorar com os amigos");
    strcpy(paginas[20].opcaoTexto[1], "Verificar se todos estao bem");
    strcpy(paginas[20].opcaoTexto[2], "Pegar os tesouros do castelo");
    strcpy(paginas[20].opcaoTexto[3], "Procurar o Cristal da Luz");
    paginas[20].opcaoDestino[0] = 22; paginas[20].opcaoDestino[1] = 22;
    paginas[20].opcaoDestino[2] = 23; paginas[20].opcaoDestino[3] = 22;
    paginas[20].opcaoMoral[0] = 1; paginas[20].opcaoMoral[1] = 2;
    paginas[20].opcaoMoral[2] = -2; paginas[20].opcaoMoral[3] = 0;
    paginas[20].opcaoCoragem[0] = 1; paginas[20].opcaoCoragem[1] = 0;
    paginas[20].opcaoCoragem[2] = 0; paginas[20].opcaoCoragem[3] = 1;

    strcpy(paginas[21].descricao,
        "A batalha foi dura, mas voces sobreviveram. Vidas foram\n"
        "perdidas, mas Arkânia esta a salvo. O castelo comeca a\n"
        "desabar. Precisam sair rapidamente!");
    strcpy(paginas[21].opcaoTexto[0], "Correr para a saida");
    strcpy(paginas[21].opcaoTexto[1], "Ajudar os feridos");
    strcpy(paginas[21].opcaoTexto[2], "Pegar o que puder");
    strcpy(paginas[21].opcaoTexto[3], "Procurar Finn desaparecido");
    paginas[21].opcaoDestino[0] = 23; paginas[21].opcaoDestino[1] = 23;
    paginas[21].opcaoDestino[2] = 23; paginas[21].opcaoDestino[3] = 24;
    paginas[21].opcaoMoral[0] = -1; paginas[21].opcaoMoral[1] = 2;
    paginas[21].opcaoMoral[2] = -2; paginas[21].opcaoMoral[3] = 1;
    paginas[21].opcaoCoragem[0] = 1; paginas[21].opcaoCoragem[1] = 0;
    paginas[21].opcaoCoragem[2] = 0; paginas[21].opcaoCoragem[3] = 1;

    strcpy(paginas[22].descricao,
        "Fora do castelo, o grupo se reune. Mestre Arkhanon aparece.\n"
        "'Voces salvaram Arkânia. O Cristal da Luz foi recuperado.'\n"
        "Ele abre um portal. E hora de voltar para casa.");
    strcpy(paginas[22].opcaoTexto[0], "Voltar para casa feliz");
    strcpy(paginas[22].opcaoTexto[1], "Se despedir de Arkânia");
    strcpy(paginas[22].opcaoTexto[2], "Pedir para ficar mais");
    strcpy(paginas[22].opcaoTexto[3], "Exigir recompensa dos elfos");
    paginas[22].opcaoDestino[0] = 25; paginas[22].opcaoDestino[1] = 24;
    paginas[22].opcaoDestino[2] = 24; paginas[22].opcaoDestino[3] = 25;
    paginas[22].opcaoMoral[0] = 0; paginas[22].opcaoMoral[1] = 1;
    paginas[22].opcaoMoral[2] = 1; paginas[22].opcaoMoral[3] = -2;
    paginas[22].opcaoCoragem[0] = 0; paginas[22].opcaoCoragem[1] = 0;
    paginas[22].opcaoCoragem[2] = 0; paginas[22].opcaoCoragem[3] = 0;

    strcpy(paginas[23].descricao,
        "O castelo desaba. Voces escapam por pouco. Finn aparece\n"
        "tossendo, mas vivo. Na distancia, veem o castelo ruir.\n"
        "Arkhanon chega voando em uma carruagem magica.");
    strcpy(paginas[23].opcaoTexto[0], "Perguntar sobre o portal");
    strcpy(paginas[23].opcaoTexto[1], "Agradecer a Arkhanon");
    strcpy(paginas[23].opcaoTexto[2], "Olhar para o castelo destruido");
    strcpy(paginas[23].opcaoTexto[3], "Abracar os amigos");
    paginas[23].opcaoDestino[0] = 25; paginas[23].opcaoDestino[1] = 24;
    paginas[23].opcaoDestino[2] = 25; paginas[23].opcaoDestino[3] = 24;
    paginas[23].opcaoMoral[0] = 0; paginas[23].opcaoMoral[1] = 1;
    paginas[23].opcaoMoral[2] = 0; paginas[23].opcaoMoral[3] = 1;
    paginas[23].opcaoCoragem[0] = 0; paginas[23].opcaoCoragem[1] = 0;
    paginas[23].opcaoCoragem[2] = 0; paginas[23].opcaoCoragem[3] = 0;

    strcpy(paginas[24].descricao,
        "Uma festa e realizada na Vila dos Elfos. Todos celebram a\n"
        "vitoria. Lyra e Naya dancem com os elfos. Finn e Kael\n"
        "compartilham historias. Torben come frutas magicas.");
    strcpy(paginas[24].opcaoTexto[0], "Fazer um discurso");
    strcpy(paginas[24].opcaoTexto[1], "Brindar com os amigos");
    strcpy(paginas[24].opcaoTexto[2], "Treinar com os elfos");
    strcpy(paginas[24].opcaoTexto[3], "Descansar e refletir");
    paginas[24].opcaoDestino[0] = 26; paginas[24].opcaoDestino[1] = 25;
    paginas[24].opcaoDestino[2] = 26; paginas[24].opcaoDestino[3] = 25;
    paginas[24].opcaoMoral[0] = 1; paginas[24].opcaoMoral[1] = 1;
    paginas[24].opcaoMoral[2] = 0; paginas[24].opcaoMoral[3] = 0;
    paginas[24].opcaoCoragem[0] = 1; paginas[24].opcaoCoragem[1] = 0;
    paginas[24].opcaoCoragem[2] = 2; paginas[24].opcaoCoragem[3] = -1;
    strcpy(paginas[25].descricao,
        "A despedida. Arkhanon abre o portal. O grupo se prepara\n"
        "para voltar. Cada um carrega memorias e experiencias.\n"
        "A amizade de voces esta mais forte do que nunca.");
    strcpy(paginas[25].opcaoTexto[0], "Ir embora sem olhar para tras");
    strcpy(paginas[25].opcaoTexto[1], "Prometer voltar um dia");
    strcpy(paginas[25].opcaoTexto[2], "Levar uma lembranca de Arkânia");
    strcpy(paginas[25].opcaoTexto[3], "Agradecer a cada amigo");
    paginas[25].opcaoDestino[0] = 27; paginas[25].opcaoDestino[1] = 27;
    paginas[25].opcaoDestino[2] = 27; paginas[25].opcaoDestino[3] = 27;
    paginas[25].opcaoMoral[0] = -1; paginas[25].opcaoMoral[1] = 1;
    paginas[25].opcaoMoral[2] = 0; paginas[25].opcaoMoral[3] = 2;
    paginas[25].opcaoCoragem[0] = 1; paginas[25].opcaoCoragem[1] = 0;
    paginas[25].opcaoCoragem[2] = 0; paginas[25].opcaoCoragem[3] = 0;

    strcpy(paginas[26].descricao,
        "A festa continua ate o amanhecer. Os elfos oferecem presentes\n"
        "a cada um de voces. Mestre Arkhanon se aproxima com um sorriso.\n"
        "'E hora de ir, jovens herois. Arkânia jamais esquecera voces.'");
    strcpy(paginas[26].opcaoTexto[0], "Ir para o portal");
    strcpy(paginas[26].opcaoTexto[1], "Pedir um presente a Arkhanon");
    strcpy(paginas[26].opcaoTexto[2], "Tirar uma foto mental");
    strcpy(paginas[26].opcaoTexto[3], "Cantar com os elfos");
    paginas[26].opcaoDestino[0] = 27; paginas[26].opcaoDestino[1] = 27;
    paginas[26].opcaoDestino[2] = 27; paginas[26].opcaoDestino[3] = 27;
    paginas[26].opcaoMoral[0] = 0; paginas[26].opcaoMoral[1] = -1;
    paginas[26].opcaoMoral[2] = 0; paginas[26].opcaoMoral[3] = 1;
    paginas[26].opcaoCoragem[0] = 1; paginas[26].opcaoCoragem[1] = 0;
    paginas[26].opcaoCoragem[2] = 0; paginas[26].opcaoCoragem[3] = 0;

    strcpy(paginas[27].descricao,
        "[FINAL - Determinado pela soma de Moral + Coragem]\n"
        "O portal se fecha. Voce acorda em sua cama. Foi tudo um\n"
        "sonho? Mas ao lado do seu travesseiro, uma pena de grifo\n"
        "e uma carta de Mestre Arkhanon provam que foi real.");
    strcpy(paginas[27].opcaoTexto[0], "Guardar a pena como tesouro");
    strcpy(paginas[27].opcaoTexto[1], "Ler a carta de Arkhanon");
    strcpy(paginas[27].opcaoTexto[2], "Ligar para os amigos");
    strcpy(paginas[27].opcaoTexto[3], "Escrever sobre a aventura");
    paginas[27].opcaoDestino[0] = 27; paginas[27].opcaoDestino[1] = 27;
    paginas[27].opcaoDestino[2] = 27; paginas[27].opcaoDestino[3] = 27;
    paginas[27].opcaoMoral[0] = 0; paginas[27].opcaoMoral[1] = 0;
    paginas[27].opcaoMoral[2] = 1; paginas[27].opcaoMoral[3] = 0;
    paginas[27].opcaoCoragem[0] = 0; paginas[27].opcaoCoragem[1] = 0;
    paginas[27].opcaoCoragem[2] = 0; paginas[27].opcaoCoragem[3] = 0;

    strcpy(paginas[28].descricao,
        "[FINAL MEDIO]\n"
        "O grupo volta para casa, mas nem todos estao la. Alguns\n"
        "amigos ficaram em Arkânia, outros nao sobreviveram.\n"
        "A vida continua, mas as marcas da aventura permanecem.");
    strcpy(paginas[28].opcaoTexto[0], "Superar e seguir em frente");
    strcpy(paginas[28].opcaoTexto[1], "Procura os amigos perdidos");
    strcpy(paginas[28].opcaoTexto[2], "Voltar a rotina normal");
    strcpy(paginas[28].opcaoTexto[3], "Contar a historia para alguem");
    paginas[28].opcaoDestino[0] = 28; paginas[28].opcaoDestino[1] = 28;
    paginas[28].opcaoDestino[2] = 28; paginas[28].opcaoDestino[3] = 28;
    paginas[28].opcaoMoral[0] = 0; paginas[28].opcaoMoral[1] = 1;
    paginas[28].opcaoMoral[2] = 0; paginas[28].opcaoMoral[3] = 0;
    paginas[28].opcaoCoragem[0] = 1; paginas[28].opcaoCoragem[1] = 0;
    paginas[28].opcaoCoragem[2] = 0; paginas[28].opcaoCoragem[3] = 0;

    strcpy(paginas[29].descricao,
        "[FINAL RUIM]\n"
        "Tudo esta perdido. Vorak venceu. Arkânia mergulhou em\n"
        "trevas eternas. Os poucos sobreviventes se escondem.\n"
        "A esperanca se foi. O peso de suas escolhas e grande.");
    strcpy(paginas[29].opcaoTexto[0], "Aceitar a derrota");
    strcpy(paginas[29].opcaoTexto[1], "Lutar ate o fim");
    strcpy(paginas[29].opcaoTexto[2], "Tentar fugir");
    strcpy(paginas[29].opcaoTexto[3], "Se render a escuridao");
    paginas[29].opcaoDestino[0] = 29; paginas[29].opcaoDestino[1] = 29;
    paginas[29].opcaoDestino[2] = 29; paginas[29].opcaoDestino[3] = 29;
    paginas[29].opcaoMoral[0] = 0; paginas[29].opcaoMoral[1] = 0;
    paginas[29].opcaoMoral[2] = -1; paginas[29].opcaoMoral[3] = -2;
    paginas[29].opcaoCoragem[0] = -1; paginas[29].opcaoCoragem[1] = 1;
    paginas[29].opcaoCoragem[2] = -1; paginas[29].opcaoCoragem[3] = -2;
}
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));
    inicializarPaginas();
    menuPrincipal();
    return 0;
}
