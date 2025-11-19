#include<raylib.h>
#include<raymath.h>

Color COULEUR_CIEL={135,206,235,255};
Color COULEUR_HERBE = {34, 139, 34, 255};
Color COULEUR_ASPHALTE = {50, 50, 50, 255};
Color COULEUR_TROTTOIRS = {224, 212, 193, 255};




void DrawRoute3Voie(){
    float largeur_route=25.0f;
    float longueur_route=50.0f;

    DrawCube((Vector3){0,0.01f,0},largeur_route,0.02f,longueur_route,COULEUR_ASPHALTE);

    //draw trottoirs
    float largeur_trottoir=1.5f;
    float hauteur_trottoir=0.2f;

    float position_trottoir_droite=largeur_route/2+largeur_trottoir/2;
    DrawCube((Vector3){position_trottoir_droite,hauteur_trottoir/2+0.01f,0},largeur_trottoir,hauteur_trottoir,longueur_route,COULEUR_TROTTOIRS);

    float position_trottoir_gauche=-largeur_route/2-largeur_trottoir/2;
    DrawCube((Vector3){position_trottoir_gauche,hauteur_trottoir/2+0.01f,0},largeur_trottoir,hauteur_trottoir,longueur_route,COULEUR_TROTTOIRS);

    //draw ligne blanches à gauche et à droite
    float largeur_ligne=0.2f;
    float hauteur_ligne=0.03f;

    float position_ligne_droite=largeur_route/2-largeur_ligne/2;
    DrawCube((Vector3){position_ligne_droite,0.02f,0},largeur_ligne,hauteur_ligne,longueur_route,WHITE);
    float position_ligne_gauche=-largeur_route/2+largeur_ligne/2;
    DrawCube((Vector3){position_ligne_gauche,0.02f,0},largeur_ligne,hauteur_ligne,longueur_route,WHITE);
    
    //draw ligne jaune au milieu
    float espace_route=1.0f;

    float position_ligne_jaune_droite=espace_route/2-largeur_ligne/2;
    DrawCube((Vector3){position_ligne_jaune_droite,0.02f,0},largeur_ligne,hauteur_ligne,longueur_route,YELLOW);
    float position_ligne_jaune_gauche=-espace_route/2+largeur_ligne/2;
    DrawCube((Vector3){position_ligne_jaune_gauche,0.02f,0},largeur_ligne,hauteur_ligne,longueur_route,YELLOW);
    
    //lignes blaches (pointillée)
    float longuer_trait=1.2f;
    float longuer_espace=1.2f;
    float longuer_segment=longuer_trait+longuer_espace;
    int nombre_segment=(int)longueur_route/(longuer_segment);

    for(int i=-nombre_segment/2;i<nombre_segment/2;i++){
        float positionZ=i * longuer_segment;
        if(positionZ>=-longueur_route/2 && positionZ<=longueur_route/2){
            DrawCube((Vector3){4.0f+espace_route/2,0.02f,positionZ},largeur_ligne,hauteur_ligne,longuer_trait,WHITE);
             DrawCube((Vector3){8.0f+espace_route/2,0.02f,positionZ},largeur_ligne,hauteur_ligne,longuer_trait,WHITE);
            DrawCube((Vector3){-4.0f-espace_route/2,0.02f,positionZ},largeur_ligne,hauteur_ligne,longuer_trait,WHITE);
            DrawCube((Vector3){-8.0f-espace_route/2,0.02f,positionZ},largeur_ligne,hauteur_ligne,longuer_trait,WHITE);
        }
    }
}



int main(){
    const int largeur_fenetre = 1400;
    const int hauteur_fenetre = 800;
    InitWindow(largeur_fenetre, hauteur_fenetre, "route_multi_voie_3D");

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        // Contrôles caméra
        if (IsKeyDown(KEY_RIGHT)) camera.position.x += 0.1f;
        if (IsKeyDown(KEY_LEFT)) camera.position.x -= 0.1f;
        if (IsKeyDown(KEY_UP)) camera.position.z -= 0.1f;
        if (IsKeyDown(KEY_DOWN)) camera.position.z += 0.1f;
        if (IsKeyDown(KEY_W)) camera.position.y += 0.1f;
        if (IsKeyDown(KEY_S)) camera.position.y -= 0.1f;
        
        BeginDrawing();
            ClearBackground(COULEUR_CIEL);
            BeginMode3D(camera);
                DrawPlane((Vector3){0, 0, 0}, (Vector2){50,50}, COULEUR_HERBE);
                DrawRoute3Voie();
               
                
            EndMode3D();
            
            DrawText("ROUTE_MULTI_VOIE_3D - Fleches pour bouger, Z/S pour monter/descendre", 10, 10, 20, BLACK);
            DrawFPS(largeur_fenetre - 100, 10);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
