#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_CONTATOS 255

struct Usuario {
    char nome[50];
    char sobrenome[50];
    char email[100];
    char telefone[20];
};


int validarEmail(const char *email) {
    const char *regexPattern = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$";
    regex_t regex;
    if (regcomp(&regex, regexPattern, REG_EXTENDED) != 0) {
        return 0; 
    }
    int result = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    return result == 0;
}

int telefoneUnico(const char *telefone, struct Usuario *listaPessoal, int numUsuariosPessoal, struct Usuario *listaTrabalho, int numUsuariosTrabalho) {
    for (int i = 0; i < numUsuariosPessoal; i++) {
        if (strcmp(listaPessoal[i].telefone, telefone) == 0) {
            return 0;
        }
    }
    for (int i = 0; i < numUsuariosTrabalho; i++) {
        if (strcmp(listaTrabalho[i].telefone, telefone) == 0) {
            return 0;
        }
    }
    return 1;
}

void adicionarUsuario(struct Usuario *lista, int *numUsuarios, struct Usuario *listaPessoal, int numUsuariosPessoal, struct Usuario *listaTrabalho, int numUsuariosTrabalho) {
    if (*numUsuarios >= MAX_CONTATOS) {
        printf("Lista de contatos cheia.\n");
        return;
    }

    struct Usuario novoUsuario;

    printf("Digite o nome do usuário: ");
    fgets(novoUsuario.nome, 50, stdin);
    novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';  

    printf("Digite o sobrenome do usuário: ");
    fgets(novoUsuario.sobrenome, 50, stdin);
    novoUsuario.sobrenome[strcspn(novoUsuario.sobrenome, "\n")] = '\0';

    do {
        printf("Digite o email do usuário: ");
        fgets(novoUsuario.email, 100, stdin);
        novoUsuario.email[strcspn(novoUsuario.email, "\n")] = '\0';
        if (!validarEmail(novoUsuario.email)) {
            printf("Email inválido. Tente novamente.\n");
        }
    } while (!validarEmail(novoUsuario.email));

    do {
        printf("Digite o telefone do usuário: ");
        fgets(novoUsuario.telefone, 20, stdin);
        novoUsuario.telefone[strcspn(novoUsuario.telefone, "\n")] = '\0';
        if (!telefoneUnico(novoUsuario.telefone, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho)) {
            printf("Telefone já existe. Tente novamente.\n");
        }
    } while (!telefoneUnico(novoUsuario.telefone, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho));

    lista[*numUsuarios] = novoUsuario;
    (*numUsuarios)++;
}

void mostrarUsuarios(struct Usuario *lista, int numUsuarios) {
    if (numUsuarios == 0) {
        printf("Lista de Usuários está vazia.\n");
        return;
    }
    printf("Lista de Usuários:\n");
    for (int i = 0; i < numUsuarios; i++) {
        printf("%d. Nome: %s\n", i+1, lista[i].nome);
        printf("   Sobrenome: %s\n", lista[i].sobrenome);
        printf("   Email: %s\n", lista[i].email);
        printf("   Telefone: %s\n", lista[i].telefone);
        printf("\n");    
    }
}

void deletarUsuarios(struct Usuario *lista, int *numUsuarios) {
    char telefone[20];
    printf("Digite o número de telefone do usuário que deseja deletar: ");
    fgets(telefone, 20, stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < *numUsuarios; i++) {
        if (strcmp(lista[i].telefone, telefone) == 0) {
            for (int j = i; j < *numUsuarios - 1; j++) {
                lista[j] = lista[j + 1];
            }
            (*numUsuarios)--;
            encontrado = 1;
            printf("Usuário deletado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Usuário não encontrado.\n");
    }
}

void alterarUsuario(struct Usuario *lista, int numUsuarios, struct Usuario *listaPessoal, int numUsuariosPessoal, struct Usuario *listaTrabalho, int numUsuariosTrabalho) {
    char telefone[20];
    printf("Digite o número de telefone do usuário que deseja alterar: ");
    fgets(telefone, 20, stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(lista[i].telefone, telefone) == 0) {
            struct Usuario alterado;

            printf("Digite o novo nome do usuário: ");
            fgets(alterado.nome, 50, stdin);
            alterado.nome[strcspn(alterado.nome, "\n")] = '\0';

            printf("Digite o novo sobrenome do usuário: ");
            fgets(alterado.sobrenome, 50, stdin);
            alterado.sobrenome[strcspn(alterado.sobrenome, "\n")] = '\0';

            do {
                printf("Digite o novo email do usuário: ");
                fgets(alterado.email, 100, stdin);
                alterado.email[strcspn(alterado.email, "\n")] = '\0';
                if (!validarEmail(alterado.email)) {
                    printf("Email inválido. Tente novamente.\n");
                }
            } while (!validarEmail(alterado.email));

            do {
                printf("Digite o novo telefone do usuário: ");
                fgets(alterado.telefone, 20, stdin);
                alterado.telefone[strcspn(alterado.telefone, "\n")] = '\0';
                if (!telefoneUnico(alterado.telefone, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho)) {
                    printf("Telefone já existe. Tente novamente.\n");
                }
            } while (!telefoneUnico(alterado.telefone, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho));

            lista[i] = alterado;
            printf("Usuário alterado com sucesso.\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Usuário não encontrado.\n");
    }
}

void salvarAgenda(struct Usuario *listaPessoal, int numUsuariosPessoal, struct Usuario *listaTrabalho, int numUsuariosTrabalho) {
    FILE *arquivo = fopen("agenda.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fwrite(&numUsuariosPessoal, sizeof(int), 1, arquivo);
    fwrite(listaPessoal, sizeof(struct Usuario), numUsuariosPessoal, arquivo);
    fwrite(&numUsuariosTrabalho, sizeof(int), 1, arquivo);
    fwrite(listaTrabalho, sizeof(struct Usuario), numUsuariosTrabalho, arquivo);
    fclose(arquivo);
    printf("Agenda salva com sucesso.\n");
}

void carregarAgenda(struct Usuario *listaPessoal, int *numUsuariosPessoal, struct Usuario *listaTrabalho, int *numUsuariosTrabalho) {
    FILE *arquivo = fopen("agenda.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fread(numUsuariosPessoal, sizeof(int), 1, arquivo);
    fread(listaPessoal, sizeof(struct Usuario), *numUsuariosPessoal, arquivo);
    fread(numUsuariosTrabalho, sizeof(int), 1, arquivo);
    fread(listaTrabalho, sizeof(struct Usuario), *numUsuariosTrabalho, arquivo);
    fclose(arquivo);
    printf("Agenda carregada com sucesso.\n");
}

int main() {
    struct Usuario listaPessoal[MAX_CONTATOS];
    struct Usuario listaTrabalho[MAX_CONTATOS];
    int numUsuariosPessoal = 0;
    int numUsuariosTrabalho = 0;
    int opcao;
    char buffer[10];  

    do {
        printf("------------------------------------------");
        printf("\nMenu:\n");
        printf("1. Adicionar Contato Pessoal\n");
        printf("2. Adicionar Contato de Trabalho\n");
        printf("3. Mostrar Contato Pessoais\n");
        printf("4. Mostrar Contato de Trabalho\n");
        printf("5. Deletar Contato Pessoal\n");
        printf("6. Deletar Contato de Trabalho\n");
        printf("7. Alterar Contato Pessoal\n");
        printf("8. Alterar Contato de Trabalho\n");
        printf("9. Salvar Agenda\n");
        printf("10. Carregar Agenda\n");
        printf("11. Sair\n");
        printf("------------------------------------------");
        printf("\nEscolha uma opção: ");
        fgets(buffer, 10, stdin);
        opcao = atoi(buffer);

        switch(opcao) {
            case 1:
                printf("\n");
                adicionarUsuario(listaPessoal, &numUsuariosPessoal, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho);
                break;
            case 2:
                printf("\n");
                adicionarUsuario(listaTrabalho, &numUsuariosTrabalho, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho);
                break;
            case 3:
                printf("\n");
                mostrarUsuarios(listaPessoal, numUsuariosPessoal);
                break;
            case 4:
                printf("\n");
                mostrarUsuarios(listaTrabalho, numUsuariosTrabalho);
                break;
            case 5:
                printf("\n");
                deletarUsuarios(listaPessoal, &numUsuariosPessoal);
                break;
            case 6:
                printf("\n");
                deletarUsuarios(listaTrabalho, &numUsuariosTrabalho);
                break;
            case 7:
                printf("\n");
                alterarUsuario(listaPessoal, numUsuariosPessoal, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho);
                break;
            case 8:
                printf("\n");
                alterarUsuario(listaTrabalho, numUsuariosTrabalho, listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho);
                break;
            case 9:
                printf("\n");
                salvarAgenda(listaPessoal, numUsuariosPessoal, listaTrabalho, numUsuariosTrabalho);
                break;
            case 10:
                printf("\n");
                carregarAgenda(listaPessoal, &numUsuariosPessoal, listaTrabalho, &numUsuariosTrabalho);
                break;
            case 11:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while(opcao != 11);

    return 0;
}