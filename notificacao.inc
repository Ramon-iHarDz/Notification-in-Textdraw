                    // Definers
#define             MAX_NOTIFICACOES        (5)

                    // Forward
forward             RetirarNotificacao(playerid, id_notificacao);
forward             AtualizarBarNotificacao(playerid, id_notificacao);

                    // Includes
#include            <YSI_Coding\y_hooks>
#include            <YSI_Data\y_iterate>

static PlayerText:TD_NT[MAX_PLAYERS][MAX_NOTIFICACOES][6];

static 
    //
    SegundosNotificacao[MAX_PLAYERS][MAX_NOTIFICACOES],
    TimeBarNotificacao[MAX_PLAYERS][MAX_NOTIFICACOES],
    BarNotificacao[MAX_PLAYERS][MAX_NOTIFICACOES],
    //
    Iterator:NotificacoesIter[MAX_PLAYERS]<MAX_NOTIFICACOES>;


hook OnGameModeInit() {
    Iter_Init(NotificacoesIter);
    return true;
}

hook OnPlayerDisconnect(playerid, reason) {
    for(new i = 0; i < MAX_NOTIFICACOES; i++)
        if(Iter_Contains(NotificacoesIter[playerid], i))
            RetirarNotificacao(playerid, i);
    return true;
}


/*
                    ooooooooo.   ooooo     ooo oooooooooo.  ooooo        ooooo   .oooooo.   
                    `888   `Y88. `888'     `8' `888'   `Y8b `888'        `888'  d8P'  `Y8b  
                    888   .d88'  888       8   888     888  888          888  888          
                    888ooo88P'   888       8   888oooo888'  888          888  888          
                    888          888       8   888    `88b  888          888  888          
                    888          `88.    .8'   888    .88P  888       o  888  `88b    ooo  
                    o888o           `YbodP'    o888bood8P'  o888ooooood8 o888o  `Y8bood8P'  
*/


public AtualizarBarNotificacao(playerid, id_notificacao) {
    if (SegundosNotificacao[playerid][id_notificacao] >= BarNotificacao[playerid][id_notificacao])
        return RetirarNotificacao(playerid, id_notificacao);
    
    SegundosNotificacao[playerid][id_notificacao] ++;
    
    PlayerTextDrawTextSize(playerid, TD_NT[playerid][id_notificacao][2], 97.000 - (97.000 / BarNotificacao[playerid][id_notificacao]) * SegundosNotificacao[playerid][id_notificacao], 5.0);
    PlayerTextDrawShow(playerid, TD_NT[playerid][id_notificacao][2]);
    return true;
}


/*
                    .oooooo..o ooooooooooooo   .oooooo.     .oooooo.   oooo    oooo o8o  .oooooo..o 
                    d8P'    `Y8 8'   888   `8  d8P'  `Y8b   d8P'  `Y8b  `888   .8P'  `YP d8P'    `Y8 
                    Y88bo.           888      888      888 888           888  d8'     '  Y88bo.      
                    `"Y8888o.       888      888      888 888           88888[           `"Y8888o.  
                        `"Y88b      888      888      888 888           888`88b.             `"Y88b 
                    oo     .d8P      888      `88b    d88' `88b    ooo   888  `88b.      oo     .d8P 
                    8""88888P'      o888o      `Y8bood8P'   `Y8bood8P'  o888o  o888o     8""88888P'  
*/


stock NT_ADD(playerid, const titulo[], const mensagem[], tempo) { // Criar notificacao
    if(playerid == INVALID_PLAYER_ID || playerid < 0) 
        return true;  
    
    if(!IsPlayerConnected(playerid)) 
        return true;                 
   
    new id_notificacao = Iter_Free(NotificacoesIter[playerid]);
    
    if(id_notificacao == ITER_NONE)
        return true;                            

    Iter_Add(NotificacoesIter[playerid], id_notificacao);

    // Criar o layout da notificacao
    CriarNotificacao(playerid, id_notificacao, titulo, mensagem);
    
    // Criar Progress Bar
    BarNotificacao[playerid][id_notificacao] = (tempo / 100);
    TimeBarNotificacao[playerid][id_notificacao] = SetTimerEx("AtualizarBarNotificacao", BarNotificacao[playerid][id_notificacao], true, "ii", playerid, id_notificacao);
    return true;
}


static stock RetirarNotificacao(playerid, id_notificacao) {
    // Para o tempo e a progressBar
    KillTimer(TimeBarNotificacao[playerid][id_notificacao]);
    
    SegundosNotificacao[playerid][id_notificacao] = 0;
    BarNotificacao[playerid][id_notificacao] = 0;

    // Destruir a notificacao da tela
    OcultarNotificacao(playerid, id_notificacao);

    Iter_Remove(NotificacoesIter[playerid], id_notificacao);
    return true;
}


/*
                       ooooo              .o.       oooooo   oooo   .oooooo.   ooooo     ooo ooooooooooooo 
                       `888'             .888.       `888.   .8'   d8P'  `Y8b  `888'     `8' 8'   888   `8 
                        888             .8"888.       `888. .8'   888      888  888       8       888      
                        888            .8' `888.       `888.8'    888      888  888       8       888      
                        888           .88ooo8888.       `888'     888      888  888       8       888      
                        888       o  .8'     `888.       888      `88b    d88'  `88.    .8'       888      
                       o888ooooood8 o88o     o8888o     o888o      `Y8bood8P'     `YbodP'        o888o     
*/


static stock Calculos_Colunas(calculos)
    return ((calculos > 0 ? (61) : (0)) * calculos);

static stock CriarNotificacao(playerid, id_notificacao, const titulo[], const mensagem[]) {
    TD_NT[playerid][id_notificacao][0] = CreatePlayerTextDraw(playerid, 540.000, 147.000 + Calculos_Colunas(id_notificacao), "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, TD_NT[playerid][id_notificacao][0], 97.000, 35.000);
    PlayerTextDrawAlignment(playerid, TD_NT[playerid][id_notificacao][0], 1);
    PlayerTextDrawColor(playerid, TD_NT[playerid][id_notificacao][0], -602653441);
    PlayerTextDrawSetShadow(playerid, TD_NT[playerid][id_notificacao][0], 0);
    PlayerTextDrawSetOutline(playerid, TD_NT[playerid][id_notificacao][0], 0);
    PlayerTextDrawBackgroundColor(playerid, TD_NT[playerid][id_notificacao][0], 255);
    PlayerTextDrawFont(playerid, TD_NT[playerid][id_notificacao][0], 4);
    PlayerTextDrawSetProportional(playerid, TD_NT[playerid][id_notificacao][0], 1);

    TD_NT[playerid][id_notificacao][1] = CreatePlayerTextDraw(playerid, 540.000, 183.601 + Calculos_Colunas(id_notificacao), "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, TD_NT[playerid][id_notificacao][1], 97.000, 5.000);
    PlayerTextDrawAlignment(playerid, TD_NT[playerid][id_notificacao][1], 1);
    PlayerTextDrawColor(playerid, TD_NT[playerid][id_notificacao][1], 320017407);
    PlayerTextDrawSetShadow(playerid, TD_NT[playerid][id_notificacao][1], 0);
    PlayerTextDrawSetOutline(playerid, TD_NT[playerid][id_notificacao][1], 0);
    PlayerTextDrawBackgroundColor(playerid, TD_NT[playerid][id_notificacao][1], 255);
    PlayerTextDrawFont(playerid, TD_NT[playerid][id_notificacao][1], 4);
    PlayerTextDrawSetProportional(playerid, TD_NT[playerid][id_notificacao][1], 1);

    TD_NT[playerid][id_notificacao][2] = CreatePlayerTextDraw(playerid, 540.000, 183.601 + Calculos_Colunas(id_notificacao), "LD_SPAC:white");// bar
    PlayerTextDrawTextSize(playerid, TD_NT[playerid][id_notificacao][2], 97.000, 5.000);
    PlayerTextDrawAlignment(playerid, TD_NT[playerid][id_notificacao][2], 1);
    PlayerTextDrawColor(playerid, TD_NT[playerid][id_notificacao][2], -602653441);
    PlayerTextDrawSetShadow(playerid, TD_NT[playerid][id_notificacao][2], 0);
    PlayerTextDrawSetOutline(playerid, TD_NT[playerid][id_notificacao][2], 0);
    PlayerTextDrawBackgroundColor(playerid, TD_NT[playerid][id_notificacao][2], 255);
    PlayerTextDrawFont(playerid, TD_NT[playerid][id_notificacao][2], 4);
    PlayerTextDrawSetProportional(playerid, TD_NT[playerid][id_notificacao][2], 1);

    TD_NT[playerid][id_notificacao][3] = CreatePlayerTextDraw(playerid, 540.000, 132.199 + Calculos_Colunas(id_notificacao), "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, TD_NT[playerid][id_notificacao][3], 97.000, 12.000);
    PlayerTextDrawAlignment(playerid, TD_NT[playerid][id_notificacao][3], 1);
    PlayerTextDrawColor(playerid, TD_NT[playerid][id_notificacao][3], -602653441);
    PlayerTextDrawSetShadow(playerid, TD_NT[playerid][id_notificacao][3], 0);
    PlayerTextDrawSetOutline(playerid, TD_NT[playerid][id_notificacao][3], 0);
    PlayerTextDrawBackgroundColor(playerid, TD_NT[playerid][id_notificacao][3], 255);
    PlayerTextDrawFont(playerid, TD_NT[playerid][id_notificacao][3], 4);
    PlayerTextDrawSetProportional(playerid, TD_NT[playerid][id_notificacao][3], 1);

    TD_NT[playerid][id_notificacao][4] = CreatePlayerTextDraw(playerid, 588.800, 133.799 + Calculos_Colunas(id_notificacao), titulo);
    PlayerTextDrawLetterSize(playerid, TD_NT[playerid][id_notificacao][4], 0.180, 0.899);
    PlayerTextDrawAlignment(playerid, TD_NT[playerid][id_notificacao][4], 2);
    PlayerTextDrawColor(playerid, TD_NT[playerid][id_notificacao][4], -1);
    PlayerTextDrawSetShadow(playerid, TD_NT[playerid][id_notificacao][4], 0);
    PlayerTextDrawSetOutline(playerid, TD_NT[playerid][id_notificacao][4], 0);
    PlayerTextDrawBackgroundColor(playerid, TD_NT[playerid][id_notificacao][4], 150);
    PlayerTextDrawFont(playerid, TD_NT[playerid][id_notificacao][4], 1);
    PlayerTextDrawSetProportional(playerid, TD_NT[playerid][id_notificacao][4], 1);

    TD_NT[playerid][id_notificacao][5] = CreatePlayerTextDraw(playerid, 589.400, 149.700 + Calculos_Colunas(id_notificacao), mensagem);
    PlayerTextDrawLetterSize(playerid, TD_NT[playerid][id_notificacao][5], 0.109, 0.699);
    PlayerTextDrawAlignment(playerid, TD_NT[playerid][id_notificacao][5], 2);
    PlayerTextDrawColor(playerid, TD_NT[playerid][id_notificacao][5], -1);
    PlayerTextDrawSetShadow(playerid, TD_NT[playerid][id_notificacao][5], 0);
    PlayerTextDrawSetOutline(playerid, TD_NT[playerid][id_notificacao][5], 0);
    PlayerTextDrawBackgroundColor(playerid, TD_NT[playerid][id_notificacao][5], 150);
    PlayerTextDrawFont(playerid, TD_NT[playerid][id_notificacao][5], 1);
    PlayerTextDrawSetProportional(playerid, TD_NT[playerid][id_notificacao][5], 1);

    for (new i = 0; i < sizeof TD_NT[][]; i++)
        PlayerTextDrawShow(playerid,TD_NT[playerid][id_notificacao][i]);
}

static stock OcultarNotificacao(playerid, id_notificacao) {
    for(new i = 0; i < sizeof TD_NT[][]; i++) {
        PlayerTextDrawDestroy(playerid, TD_NT[playerid][id_notificacao][i]);
        TD_NT[playerid][id_notificacao][i] = PlayerText:INVALID_TEXT_DRAW;
    }
}


CMD:testen(playerid, const params[]) {
    static 
        titulo[10],
        mensagem[255],
        tempo;

    if (sscanf(params, "s[10]s[255]i", titulo, mensagem, tempo))
        return SendClientMessage(playerid, -1, "Digite /testen [titulo] [mensagem] [tempo]");

    NT_ADD(playerid, titulo, mensagem, tempo);
    return true;
}
