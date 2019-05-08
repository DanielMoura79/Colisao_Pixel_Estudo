#include <allegro.h>

// DECLARACOES INICIAIS //
int sair=0; void sair_allegro() {sair=1;}; 
int timer=0; void tempo() {timer++;};
int Ctrl_FPS=60; float delay=0;

// INICIALIZACAO ALLEGRO //
int main()
{
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse(); 
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
  install_int_ex(tempo, BPS_TO_TIMER(60)); //60fps
  set_window_title("ALLEGRO MINIMAL");
  set_close_button_callback( sair_allegro );
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  BITMAP *buffer = create_bitmap(640,480);
  BITMAP *player = load_bitmap("player.pcx",NULL);
  BITMAP *AreaRecorte1 = create_bitmap(player->w,player->h);
  BITMAP *AreaRecorte2 = create_bitmap(player->w,player->h);
  clear_to_color(AreaRecorte1, makecol(255,000,255));
  clear_to_color(AreaRecorte2, makecol(255,000,255));
  BITMAP *obstaculo = load_bitmap("obstaculo.pcx",NULL);
  int player_x1=50;
  int player_y1=50;
  int player_x2=player_x1+player->w;
  int player_y2=player_y1+player->h;
  int obstaculo_x1=190-50;
  int obstaculo_y1=280-50;
  int obstaculo_x2=obstaculo_x1+obstaculo->w;
  int obstaculo_y2=obstaculo_y1+obstaculo->h;
	int PontoInterseccao1_x;
  int PontoInterseccao1_y;
  int PontoInterseccao2_x;
	int PontoInterseccao2_y;
	int AR1x1, AR1y1, AR1x2, AR1y2;
	int AR2x1, AR2y1, AR2x2, AR2y2;
  
  // LOOP DE JOGO // 
  while (sair==0)
  {
  delay=timer; 
	if (key[KEY_ESC]) {sair=1;}; 
	clear_to_color(buffer,makecol(100,149,237)); //fundo azul
	
	/* < SEU CODIGO AQUI! > */
	//inputs
	if (key[KEY_UP]) { player_y1-=2; }
	if (key[KEY_DOWN]) { player_y1+=2; }
	if (key[KEY_LEFT]) { player_x1-=2; }
	if (key[KEY_RIGHT]) { player_x1+=2; }
	
	//recalcula o x2 e y2 dos objetos
	player_x2=player_x1+player->w;
  player_y2=player_y1+player->h;
	obstaculo_x2=obstaculo_x1+obstaculo->w;
  obstaculo_y2=obstaculo_y1+obstaculo->h;
	
	/////////////////////////////////////////////////////
	// Calcula a colisao - etapa1 (colisao retangular) //
	/////////////////////////////////////////////////////
	int colisao_etapa1=0;
	int colisao_etapa2=0;
	int Ax1=player_x1;
	int Ay1=player_y1;
	int Ax2=player_x2;
	int Ay2=player_y2;
	int Bx1=obstaculo_x1;
	int By1=obstaculo_y1;
	int Bx2=obstaculo_x2;
	int By2=obstaculo_y2;
	if (
	(Ax1+(Ax2-Ax1)>Bx1) and
  (Ax1<Bx1+(Bx2-Bx1)) and 
  (Ay1+(Ay2-Ay1)>By1) and 
  (Ay1<By1+(By2-By1)) )
  { colisao_etapa1=1; } else { colisao_etapa1=0; };
  
  if(colisao_etapa1==1){
		//calcula Pontos de Interseccao
		PontoInterseccao1_x=player_x1; PontoInterseccao1_y=player_y1; 
		PontoInterseccao2_x=player_x2; PontoInterseccao2_y=player_y2; 
		if(obstaculo_x1>player_x1) { PontoInterseccao1_x=obstaculo_x1; PontoInterseccao1_y=obstaculo_y1; }
		else{ PontoInterseccao1_x=player_x1; PontoInterseccao1_y=obstaculo_y1; }
		if(player_x2>obstaculo_x2) { PontoInterseccao2_x=obstaculo_x2; PontoInterseccao2_y=player_y2;  }
		if(player_y1>obstaculo_y1) { PontoInterseccao1_y=player_y1; }
		if(player_y2>obstaculo_y2) { PontoInterseccao2_y=obstaculo_y2; }	
		
		///////////////////////////////////////////////////
		// Calcula a colisao - etapa2 (colisao de pixel) //
		///////////////////////////////////////////////////
		
		//tamanho da Area de Recorte
		AR1x1=PontoInterseccao1_x-player_x1;
		AR1y1=PontoInterseccao1_y-player_y1;
		AR1x2=PontoInterseccao2_x-player_x1;
		AR1y2=PontoInterseccao2_y-player_y1;
		AR2x1=PontoInterseccao1_x-obstaculo_x1;
		AR2y1=PontoInterseccao1_y-obstaculo_y1;
		AR2x2=PontoInterseccao2_x-obstaculo_x1;
		AR2y2=PontoInterseccao2_y-obstaculo_y1;
		AreaRecorte1->w=AR1x2-AR1x1;
		AreaRecorte1->h=AR1y2-AR1y1;
		AreaRecorte2->w=AR2x2-AR2x1;
		AreaRecorte2->h=AR2y2-AR2y1;
		//abastece areas de recorte
		clear_to_color(AreaRecorte1, makecol(255,000,255));
		clear_to_color(AreaRecorte2, makecol(255,000,255));
		blit(player, AreaRecorte1, AR1x1, AR1y1, 0, 0, AR1x2, AR1y2);
		blit(obstaculo, AreaRecorte2, AR2x1, AR2y1, 0, 0, AR2x2, AR2y2);
		
		//verifica colisao por pixel, na area de recorte
		int x1=0;
		int x2=AreaRecorte1->w;
		int y1=0;
		int y2=AreaRecorte1->h;
		for(y1 = 0; y1 < y2; y1++) { 
			for(x1 = 0; x1 < x2; x1++) {
				int contador=0;
				if(getpixel(AreaRecorte1, x1, y1) != makecol( 255, 000, 255)) { contador++; }
				if(getpixel(AreaRecorte2, x1, y1) != makecol( 255, 000, 255)) { contador++; }
				if(contador==2) { colisao_etapa2=1; break; }
			}
		}            	            
	}
	
	//reseta interseccao em caso de nao colisao
	if(colisao_etapa1==0) {
		PontoInterseccao1_x=-1; 
		PontoInterseccao1_y=-1; 
		PontoInterseccao2_x=-1; 
		PontoInterseccao2_y=-1; 
		AreaRecorte1->w=0;
		AreaRecorte1->h=0;
		AreaRecorte2->w=0;
		AreaRecorte2->h=0;
	}
  
	//desenha objetos e retangulos
	if(colisao_etapa2==1)rectfill(buffer, PontoInterseccao1_x, PontoInterseccao1_y, PontoInterseccao2_x, PontoInterseccao2_y, makecol(255,0,0));
	draw_sprite(buffer, obstaculo, obstaculo_x1, obstaculo_y1);
  rect(buffer, obstaculo_x1, obstaculo_y1, obstaculo_x2, obstaculo_y2, makecol(000,000,255));  
	draw_sprite(buffer, player, player_x1, player_y1);
  rect(buffer, player_x1, player_y1, player_x2, player_y2, makecol(255,000,000));
  draw_sprite(buffer, AreaRecorte1, buffer->w-AreaRecorte2->w*2, 0);
  rect(buffer, buffer->w-AreaRecorte1->w*2, 0, (buffer->w-AreaRecorte1->w*2)+AreaRecorte1->w, AreaRecorte1->h, makecol(255,000,000));
  draw_sprite(buffer, AreaRecorte2, buffer->w-AreaRecorte2->w, 0);
  rect(buffer, buffer->w-AreaRecorte2->w, 0, (buffer->w-AreaRecorte2->w)+AreaRecorte2->w, AreaRecorte2->h, makecol(000,000,255));
  //desenha interseccao
  rect(buffer, PontoInterseccao1_x, PontoInterseccao1_y, PontoInterseccao2_x, PontoInterseccao2_y, makecol(255,255,255));
	if(colisao_etapa2==1){
		circle(buffer, PontoInterseccao1_x, PontoInterseccao1_y, 3, makecol(255,255,255));
		circle(buffer, PontoInterseccao2_x, PontoInterseccao1_y, 3, makecol(255,255,255));
	  circle(buffer, PontoInterseccao2_x, PontoInterseccao2_y, 3, makecol(255,255,255));
	  circle(buffer, PontoInterseccao1_x, PontoInterseccao2_y, 3, makecol(255,255,255));
	}
	  
  //desenha debug na tela
	textprintf_ex(buffer, font, 5,  5, makecol(255,255,255), -1, "Colisao Inteligente de Pixel");
  textprintf_ex(buffer, font, 5, 25, makecol(255,255,255), -1, "Primeiro verifica a colisao entre Retangulos, ");
	textprintf_ex(buffer, font, 5, 45, makecol(255,255,255), -1, "em seguida, verifica colisao de pixel na interseccao");
	if(colisao_etapa1==1) { textprintf_ex(buffer, font, 5,  85, makecol(255,255,000), -1, "COLISAO RETANGULAR DETECTADA!!! (Broad Phase)"); }
	if(colisao_etapa2==1) { textprintf_ex(buffer, font, 5, 105, makecol(255,255,000), -1, "COLISAO DE PIXEL DETECTADA!!! (Narrow Phase)"); }
	draw_sprite(screen, buffer, 0, 0);
		
  // FINALIZACOES //
	while(timer==delay){};	
	clear(buffer);
  }; 
  destroy_bitmap(buffer);
  return 0;
};
END_OF_MAIN();
