// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef AMT2018_EVENTS_HPP
#define AMT2018_EVENTS_HPP

namespace amt2018 {

enum class event {
emergencia_medica = 1100,
disparo_ou_panico_de_incendio = 1110,
panico_audivel_ou_silencioso = 1120,
senha_de_coacao = 1121,
panico_silencioso = 1122,
disparo_de_zona = 1130,
disparo_de_cerca_eletrica = 1131,
disparo_de_zona_24h = 1133,
tamper_do_teclado = 1145,
disparo_silencioso = 1146,
falha_da_supervisao_smart = 1147,
sobrecarga_na_saida_auxiliar = 1300,
falha_na_rede_eletrica = 1301,
bateria_principal_baixa_ou_em_curto_circuito = 1302,
reset_pelo_modo_de_programacao = 1305,
alteracao_da_programacao_do_painel = 1306,
bateria_principal_ausente_ou_invertida = 1311,
corte_ou_curto_circuito_na_sirene = 1321,
toque_de_porteiro = 1322,
problema_em_teclado_ou_receptor = 1333,
falha_na_linha_telefonica = 1351,
falha_ao_comunicar_evento = 1354,
corte_da_fiacao_dos_sensores = 1371,
curto_circuito_na_fiacao_dos_sensores = 1372,
tamper_do_sensor = 1383,
bateria_baixa_de_sensor_sem_fio = 1384,
desativacao_pelo_usuario = 1401,
auto_desativacao = 1403,
desativacao_via_computador_ou_telefone = 1407,
acesso_remoto_pelo_software_de_download_upload = 1410,
falha_no_download = 1413,
acionamento_de_pgm = 1422,
senha_incorreta = 1461,
anulacao_temporaria_de_zona = 1570,
anulacao_por_disparo = 1573,
teste_manual = 1601,
teste_periodico = 1602,
solicitacao_de_manutencao = 1616,
reset_do_buffer_de_eventos = 1621,
log_de_eventos_cheio = 1624,
data_e_hora_foram_reiniciadas = 1625,
restauracao_de_incendio = 3110,
restauracao_disparo_de_zona = 3130,
restauracao_de_disparo_de_cerca_eletrica = 3131,
restaraucao_disparo_de_zona_24h = 3133,
restaraucao_tamper_do_teclado = 3145,
restaraucao_disparo_silencioso = 3146,
restaraucao_da_supervisao_smart = 3147,
restaraucao_sobrecarga_na_saida_auxiliar = 3300,
restaraucao_falha_na_rede_eletrica = 3301,
restaraucao_bat_princ_baixa_ou_em_curto_circuito = 3302,
restaraucao_bat_princ_ausente_ou_invertida = 3311,
restaraucao_corte_ou_curto_circuito_na_sirene = 3321,
restaraucao_problema_em_teclado_ou_receptor = 3333,
restaraucao_linha_telefonica = 3351,
restaraucao_corte_da_fiacao_dos_sensores = 3371,
restaraucao_curto_circuito_na_fiacao_dos_sensores = 3372,
restaraucao_tamper_do_sensor = 3383,
restaraucao_bateria_baixa_de_sensor_sem_fio = 3384,
ativacao_pelo_usuario = 3401,
auto_ativacao = 3403,
ativacao_via_computador_ou_telefone = 3407,
ativacao_por_uma_tecla = 3408,
desacionamento_de_pgm = 3422,
ativacao_parcial = 3456,
keep_alive = -2

};

const char* message_event(event e)
{
  switch(static_cast<int>(e))
  {
  case 1100:
    return "Emergência Médica";
  case 1110:
    return "Disparo ou pânico de incêndio";
  case 1120:
    return "Pânico audível ou silencioso";
  case 1121:
    return "Senha de coação";
  case 1122:
    return "Pânico silencioso";
  case 1130:
    return "Disparo de zona";
  case 1131:
    return "Disparo de cerca elétrica";
  case 1133:
    return "Disparo de zona 24h";
  case 1145:
    return "Tamper do teclado";
  case 1146:
    return "Disparo silencioso";
  case 1147:
    return "Falha da supervisão Smart";
  case 1300:
    return "Sobrecarga na saída auxiliar";
  case 1301:
    return "Falha na rede elétrica";
  case 1302:
    return "Bateria principal baixa ou em curto-circuito";
  case 1305:
    return "Reset pelo modo de programação";
  case 1306:
    return "Alteração da programação do painel";
  case 1311:
    return "Bateria principal ausente ou invertida";
  case 1321:
    return "Corte ou curto-circuito na sirene";
  case 1322:
    return "Toque de porteiro";
  case 1333:
    return "Problema em teclado ou receptor";
  case 1351:
    return "Falha na linha telefônica";
  case 1354:
    return "Falha ao comunicar evento";
  case 1371:
    return "Corte da fiação dos sensores";
  case 1372:
    return "Curto-circuito na fiação dos sensores";
  case 1383:
    return "Tamper do sensor";
  case 1384:
    return "Bateria baixa de sensor sem fio";
  case 1401:
    return "Desativação pelo usuário";
  case 1403:
    return "Auto-desativação";
  case 1407:
    return "Desativação via computador ou telefone";
  case 1410:
    return "Acesso remoto pelo software de download/upload";
  case 1413:
    return "Falha no download";
  case 1422:
    return "Acionamento de PGM";
  case 1461:
    return "Senha incorreta";
  case 1570:
    return "Anulação temporária de zona";
  case 1573:
    return "Anulação por disparo";
  case 1601:
    return "Teste manual";
  case 1602:
    return "Teste periódico";
  case 1616:
    return "Solicitação de manutenção";
  case 1621:
    return "Reset do buffer de eventos";
  case 1624:
    return "Log de eventos cheio";
  case 1625:
    return "Data e hora foram reiniciadas";
  case 3110:
    return "Restauração de incêndio";
  case 3130:
    return "Restauração disparo de zona";
  case 3131:
    return "Restauração de disparo de cerca elétrica";
  case 3133:
    return "Restauração disparo de zona 24h";
  case 3145:
    return "Restauração tamper do teclado";
  case 3146:
    return "Restauração disparo silencioso";
  case 3147:
    return "Restauração da supervisão Smart";
  case 3300:
    return "Restauração sobrecarga na saída auxiliar";
  case 3301:
    return "Restauração falha na rede elétrica";
  case 3302:
    return "Restauração bat. princ. baixa ou em curto-circuito";
  case 3311:
    return "Restauração bat. princ. ausente ou invertida";
  case 3321:
    return "Restauração corte ou curto-circuito na sirene";
  case 3333:
    return "Restauração problema em teclado ou receptor";
  case 3351:
    return "Restauração linha telefônica";
  case 3371:
    return "Restauração corte da fiação dos sensores";
  case 3372:
    return "Restauração curto-circuito na fiação dos sensores";
  case 3383:
    return "Restauração tamper do sensor";
  case 3384:
    return "Restauração bateria baixa de sensor sem fio";
  case 3401:
    return "Ativação pelo usuário";
  case 3403:
    return "Auto-ativação";
  case 3407:
    return "Ativação via computador ou telefone";
  case 3408:
    return "Ativação por uma tecla";
  case 3422:
    return "Desacionamento de PGM";
  case 3456:
    return "Ativação parcial";
  case -2:
    return "Keep-Alive";
  default:
    return "Unknown";
  }
  return "Unknown";
}

}

#endif
