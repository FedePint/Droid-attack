#include "SceneRenderer.h"
#include "Globals.h"
#include "Physics.h"
#include "Player.h"
#include "Enemy.h"
#include "RenderUtils.h"
#include <iostream>

void RenderScene(const GameResources& res, int& roomActive, int& roomPrev, const glm::mat4& view, const glm::mat4& projection)
{
        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render bullet
		
        int num_avaiable = 4;

        // don't forget to enable shader before setting uniforms
        res.ourShader->use();
        res.ourShader->setVec3("light.position", lightPos);
        res.ourShader->setVec3("viewPos", camera.Position);

        res.ourShader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        res.ourShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        res.ourShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        res.ourShader->setFloat("material.shininess", 64.0f);
        res.ourShader->setMat4("projection", projection);
        res.ourShader->setMat4("view", view);

       
        
        
        // render the room
        res.roomShader->use();
        glm::mat4 room = glm::mat4(1.0f);
        // translate it down so it's at the center of the scene
        room = glm::scale(room, glm::vec3(1.0f, 1.0f, 1.0f));
        room = glm::rotate(room, glm::radians(-90.0f), glm::vec3(0, 1, 0));// it's a bit too big for our scene, so scale it down
        res.roomShader->setMat4("model", room);
        res.roomShader->setMat4("view", view);
        res.roomShader->setMat4("projection", projection);
        res.roomShader->setVec3("light.position", lightPos);
        res.roomShader->setVec3("viewPos", camera.Position);
        res.roomShader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        res.roomShader->setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
        res.roomShader->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

        res.roomShader->setFloat("material.shininess", 14.0f);
        res.room->Draw(*res.roomShader);


        

        // --- INIZIO RAYCASTING DEL MOUSE ---
        // 1. Ottieni la coordinata Y del mouse (invertita, perché OpenGL va dal basso verso l'alto)
        float mouseY_OpenGL = SCR_HEIGHT - lastY;
        glm::vec4 viewport = glm::vec4(0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);

        // 2. Calcola un punto sul "Near Plane" (vicino allo schermo)
        glm::vec3 screenPosNear = glm::vec3(lastX, mouseY_OpenGL, 0.0f);
        glm::vec3 worldPosNear = glm::unProject(screenPosNear, view, projection, viewport);

        // 3. Calcola un punto sul "Far Plane" (lontano nello spazio 3D)
        glm::vec3 screenPosFar = glm::vec3(lastX, mouseY_OpenGL, 1.0f);
        glm::vec3 worldPosFar = glm::unProject(screenPosFar, view, projection, viewport);

        // 4. Crea un vettore direzione per il raggio
        glm::vec3 rayDir = glm::normalize(worldPosFar - worldPosNear);

        // 5. Intersezione con il pavimento (Piano dove Y = 0)
        // Equazione della retta: P = Pos + Direzione * t. Vogliamo che P.y sia 0.
        // 0 = worldPosNear.y + rayDir.y * t  =>  t = -worldPosNear.y / rayDir.y
        if (rayDir.y != 0.0f) { // Evita divisioni per zero se guardi perfettamente dritto
            float t = -worldPosNear.y / rayDir.y;
            mousePos = worldPosNear + rayDir * t; // Questa è la posizione esatta 3D del mouse sul pavimento!
        }

        // 6. Calcola l'angolo in cui il robot deve girarsi
        mouseDir = glm::normalize(mousePos - position);

        // Calcolo dell'angolo usando arcotangente (atan2) su assi X e Z
        robotAngle = glm::degrees(std::atan2(mouseDir.x, mouseDir.z))-90;
        // NOTA: A seconda dell'orientamento originale del tuo modello .obj, 
        // potresti dover aggiungere o sottrarre un offset (es. +90, +180, -90). 
        // Lascia robotAngle così com'è o aggiungi l'offset che ti serve per far combaciare la "faccia" del robot.
        // --- FINE RAYCASTING ---




        if (robotLifePoints <= 0)
        {

            
            roomActive = 0;
            position = glm::vec3(0.0f, 0.0f, 0.0f);
			robotLifePoints = 15;
            enemyPosition = glm::vec3(0.0f, 0.0f, -50.0f);

            for (int i = 0; i < bullets.size(); i++)
            {
                bullets[i].active = false;
                bullets[i].position = position;
            }

            for (int i = 0; i < 4; i++)
            {
                cannonLives[i] = 2;
			}

            for (int i = 0; i < 6; i++)
            {
                cannonLivesUP[i] = 2;
            }

            BossPosition = glm::vec3(0.0f, 0.0f, roomSize.z);
          

            for (int i = 0; i < Enemybullets.size(); i++)
            {
                Enemybullets[i].active = false;
                Enemybullets[i].position = BossPosition;
            }
            currentState = -1;
            return;

		}

        if (roomActive == 0)
        {

			
            // render the loaded model
          
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(robotAngle), dir);
            res.ourShader->setMat4("model", model);
            res.ourModel->Draw(*res.ourShader);

            BulletActive(bullets);
            
            for (int j = 0; j < bullets.size(); j++)
            {
                if (bullets[j].active)
                {
                    if (hitbox(bullets[j].position, 1.0f, enemyPosition, 2.0f))
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        enemyLife = false;
                        
                    }
                    else
                    {
                        //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                        res.bulletShader->use();
                        glm::mat4 bulletMatrix = glm::mat4(1.0f);
                        bulletMatrix = glm::translate(bulletMatrix, bullets[j].position);
                        bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                        res.bulletShader->setMat4("model", bulletMatrix);
                        res.bulletShader->setMat4("projection", projection);
                        res.bulletShader->setMat4("view", view);
                        res.bulletModel->Draw(*res.bulletShader);
                        num_avaiable--;
                    }
                }

            }

            
          

          

           
            if (hitbox(position, 2.0f, res.exitRoom1R, 2.0f))
            {
                roomActive = 1;
                position = -position -glm::vec3(1.0f, 0.0f, 0.0f);


				enemyPosition = glm::vec3(0.0f, 0.0f, -50.0f);
                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
				}
			}
            

            if (hitbox(position, 2.0f, -res.exitRoom1R, 2.0f))
            {
                roomActive = 4;
                speed = 45.0f;
                position = -position + glm::vec3(1.0f, 0.0f, 0.0f);
                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
                }

                for (int i = 0; i < Enemybullets.size(); i++)
                {
                    Enemybullets[i].active = false;
                    Enemybullets[i].position = BossPosition;
                }

            }
            if (hitbox(position, 2.0f, res.exitRoom2UP, 2.0f))
            {
                roomActive = 3;
                speed = 45.0f;
                position = -position + glm::vec3(0.0f, 0.0f, 1.0f);;


                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
                }

                for (int i = 0; i < Enemybullets.size(); i++)
                {
                    Enemybullets[i].active = false;
                    Enemybullets[i].position = BossPosition;
                }
            }

			RenderPorta(res.exitRoom1R, view,projection,*res.portalShader, *res.portalModel, roomSize);
			RenderPorta(-res.exitRoom1R, view, projection, *res.portalShader, *res.portalModel, roomSize);
			RenderPorta(res.exitRoom2UP, view, projection, *res.portalShader, *res.portalModel, roomSize);

        }

        else if (roomActive == 4)
        {

            res.puddleShader->use();
            glm::mat4 puddle= glm::mat4(1.0f);
           
            res.puddleShader->setMat4("model", room);
            res.puddleShader->setMat4("view", view);
            res.puddleShader->setMat4("projection", projection);
            res.puddleShader->setVec3("light.position", lightPos);
            res.puddleShader->setVec3("viewPos", camera.Position);
            res.puddleShader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
            res.puddleShader->setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
            res.puddleShader->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

            res.puddleShader->setFloat("material.shininess", 14.0f);
            res.puddleModel->Draw(*res.puddleShader);

            // render the loaded model

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(robotAngle), dir);
            res.ourShader->setMat4("model", model);
            res.ourModel->Draw(*res.ourShader);

            BulletActive(bullets);

            for (int j = 0; j < bullets.size(); j++)
            {
                if (bullets[j].active)
                {   


                    if (hitbox(bullets[j].position, 1.0f, enemyPositions[0], 2.0f) and enemyLives[0])
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        enemyLives [0] = false;
						enemykilled++;
                    }

                    else if (hitbox(bullets[j].position, 1.0f, enemyPositions[1], 2.0f) and enemyLives[1])
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        enemyLives[1] = false;
						enemykilled++;
					}

                    else if (hitbox(bullets[j].position, 1.0f, enemyPositions[2], 2.0f) and enemyLives[2])
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        enemyLives[2] = false;
						enemykilled++;
                    }

                    else
                    {
                        //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                        res.bulletShader->use();
                        glm::mat4 bulletMatrix = glm::mat4(1.0f);
                        bulletMatrix = glm::translate(bulletMatrix, bullets[j].position);
                        bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                        res.bulletShader->setMat4("model", bulletMatrix);
                        res.bulletShader->setMat4("projection", projection);
                        res.bulletShader->setMat4("view", view);
                        res.bulletModel->Draw(*res.bulletShader);
                        num_avaiable--;
                    }
                }

            }
            for (int i = 0; i < 3; i++)
            {
                if (hitbox(enemyPositions[i], 2.0f, position, 2.0f) and enemyLives[i])
                {
                    robotLifePoints = robotLifePoints - 4;
                    enemyLives[i] = false;
                }
                if (enemyLives[i])
                {   
					bool conP = false;
                    for (int j = 0; j < 3; j++)
                    {
                        if (j != i)
                        {
                            if (hitbox(enemyPositions[i], 2.5f, enemyPositions[j], 2.5f))
                            {   
								conP = true;
                                float dx = enemyPositions[j].x - enemyPositions[i].x;
                                float dz = enemyPositions[j].z - enemyPositions[i].z;
                                float distanceQuad = dx * dx + dz * dz;

                                float minDistance = 2.5f * 2.0f; // Somma dei due raggi

                                // Se la distanza al quadrato è minore del limite consentito, stanno collidendo
                                
                                    float distance = std::sqrt(distanceQuad);
                                    float overlap = minDistance - distance;

                                    // Trova la direzione in cui spingerli (normalizzazione)
                                    float nx = dx / distance;
                                    float nz = dz / distance;

                                    // Sposta il nemico 'i' un po' indietro e il nemico 'j' un po' in avanti
                                    // (Ognuno si fa carico di metà della sovrapposizione)
                                    enemyPositions[i].x -= nx * (overlap * 0.5f);
                                    enemyPositions[i].z -= nz * (overlap * 0.5f);

                                    enemyPositions[j].x += nx * (overlap * 0.5f);
                                    enemyPositions[j].z += nz * (overlap * 0.5f);
                            }
                        }
                    }
                    if (conP)
                    {
                        enemySpeed = enemySpeed * 1, 60;
                    }

                    enemyChasing(position, enemyPositions[i], enemyAngles[i]);
                    enemySpeed = enemySpeed /1, 60;
                    
                    res.enemyShader->use();
                    res.enemyShader->setVec3("light.position", lightPos);
                    res.enemyShader->setVec3("viewPos", camera.Position);

                    res.enemyShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
                    res.enemyShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
                    res.enemyShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                    res.enemyShader->setFloat("material.shininess", 80.0f);

                    glm::mat4 enemyModelMatrix = glm::mat4(1.0f);
                    enemyModelMatrix = glm::translate(enemyModelMatrix, enemyPositions[i]); // translate it down so it's at the center of the scene
                    enemyModelMatrix = glm::scale(enemyModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));// it's a bit too big for our scene, so scale it down
                    enemyModelMatrix = glm::rotate(enemyModelMatrix, glm::radians(enemyAngles[i] - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    res.enemyShader->setMat4("model", enemyModelMatrix);
                    res.enemyShader->setMat4("view", view);
                    res.enemyShader->setMat4("projection", projection);
                    res.enemyModel->Draw(*res.enemyShader);
                }

             
            }
            
            


            


            if (!SpeedUp)
            {   
                res.gemShader->use();
                glm::mat4 gem = glm::mat4(1.0f);
				glm::vec3 gemPos =(-res.exitRoom1R) - glm::vec3(2.0f, 0.0f, 0.0f);
                gem = glm::translate(gem, gemPos);
				gem = glm::scale(gem, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
                res.gemShader->setMat4("model", gem);
                res.gemShader->setMat4("view", view);
                res.gemShader->setMat4("projection", projection);
                res.gemShader->setVec3("light.position", lightPos);
                res.gemShader->setVec3("viewPos", camera.Position);
                res.gemShader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
                res.gemShader->setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
                res.gemShader->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

                res.gemShader->setFloat("material.shininess", 14.0f);
                res.gemModel->Draw(*res.gemShader);


                if ((hitbox(glm::vec3(roomSize.x / 2.0f, 0.0f, roomSize.z / 4.0f), roomSize.z / 2.0f, position, 0.5) or hitbox(glm::vec3(roomSize.x / 2.0f, 0.0f, -roomSize.z / 2.0f), roomSize.z / 2.0f, position, 0.5)
                    or hitbox(glm::vec3(0.0f, 0.0f, -roomSize.z / 2.0f), roomSize.z / 4.0f, position, 0.5)))
                {
                    moveSpeed = 15.0f;
                }
                else
                {
                    moveSpeed = 30.0f;
                }

                if (hitbox(position, 2.0f, -res.exitRoom1R, 2.0f))
                {
                    SpeedUp = true;
                    moveSpeed =45.0f;
                }
            }

            if (hitbox(position, 2.0f, res.exitRoom1R, 2.0f))
            {
                roomActive = 0;
                position = -position - glm::vec3(1.0f, 0.0f, 0.0f);
                if (SpeedUp)
                {
                    moveSpeed = 45.0f;
                }
                else
                {
                    moveSpeed = 30.0f;
                }

                enemyPosition = glm::vec3(0.0f, 0.0f, -50.0f);
                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
                }

                for (int i = 0; i < Enemybullets.size(); i++)
                {
                    if (i == 0)
                    {
                        enemyPositions[i] = glm::vec3(roomSize.x - 8.0f, 0.0f, -roomSize.z + 8.0f);

                    }
                    else if (i == 1)
                    {
                        enemyPositions[i] = glm::vec3(0.0f, 0.0f, 0.0f);
                    }
                    else if (i == 2)
                    {
                        enemyPositions[i] = glm::vec3(roomSize.x - 8.0f, 0.0f, roomSize.z - 8.0f);
                    }
                }
            }

            RenderPorta(res.exitRoom1R, view, projection, *res.portalShader, *res.portalModel, roomSize);
           



        }

        else if (roomActive == 3)
        {
            speed = 45.0f;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(robotAngle), dir);
            res.ourShader->setMat4("model", model);
            res.ourModel->Draw(*res.ourShader);

            BulletActive(bullets);
            speed = 10.0f;

            for (int j = 0; j < bullets.size(); j++)
            {
                if (bullets[j].active)
                {


                    if (hitbox(bullets[j].position, 1.0f, cannonPositionsUP[0], 4.0f) && cannonLivesUP[0] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLivesUP[0]--;
                        if (cannonLivesUP[0] <= 0)
                        {
							enemykilled++;
						}
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositionsUP[1], 4.0f) && cannonLivesUP[1] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLivesUP[1]--;
                        if (cannonLivesUP[1] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositionsUP[2], 4.0f) && cannonLivesUP[2] > 0)
                    {
                        bullets[j].active = false;
                        cannonLivesUP[2]--;
                        if (cannonLivesUP[2] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositionsUP[3], 4.0f) && cannonLivesUP[3] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLivesUP[3]--;
                        if (cannonLivesUP[3] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositionsUP[4], 4.0f) && cannonLivesUP[4] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLivesUP[4]--;
                        if (cannonLivesUP[4] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositionsUP[5], 4.0f) && cannonLivesUP[5] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLivesUP[5]--;
                        if (cannonLivesUP[5] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else
                    {
                        //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                        res.bulletShader->use();
                        glm::mat4 bulletMatrix = glm::mat4(1.0f);
                        bulletMatrix = glm::translate(bulletMatrix, bullets[j].position);
                        bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                        res.bulletShader->setMat4("model", bulletMatrix);
                        res.bulletShader->setMat4("projection", projection);
                        res.bulletShader->setMat4("view", view);
                        res.bulletModel->Draw(*res.bulletShader);
                        num_avaiable--;
                    }
                }

            }


            // render cannon


            for (int i = 0; i < 6; i++)
            {

                if (i%2 == 0)
                {
					cannonAngle = 00.0f;
                }
                else
                {
					cannonAngle = -180.0f;
				}

                if (cannonLivesUP[i] > 0)
                {

					
                    res.cannonShader->use();
                    res.cannonShader->setVec3("light.position", lightPos);
                    res.cannonShader->setVec3("viewPos", camera.Position);

                    res.cannonShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
                    res.cannonShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
                    res.cannonShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                    res.cannonShader->setFloat("material.shininess", 80.0f);

                    glm::mat4 cannonModelMatrix = glm::mat4(1.0f);
                    cannonModelMatrix = glm::translate(cannonModelMatrix, cannonPositionsUP[i]); // translate it down so it's at the center of the scene
                    cannonModelMatrix = glm::scale(cannonModelMatrix, glm::vec3(8.0f, 8.0f, 8.0f));// it's a bit too big for our scene, so scale it down
                    cannonModelMatrix = glm::rotate(cannonModelMatrix, glm::radians(cannonAngle), glm::vec3(0.0f, 1.0f, 0.0f));
                    
                    res.cannonShader->setMat4("model", cannonModelMatrix);
                    res.cannonShader->setMat4("view", view);
                    res.cannonShader->setMat4("projection", projection);
                    res.cannonModel->Draw(*res.cannonShader);


                    BulletActive(Enemybullets);
                    // cannon shoot
                    if (Enemybullets[i].active)
                    {
                        if (hitbox(Enemybullets[i].position, 1.0f, position, 2.0f))
                        {
                            Enemybullets[i].active = false;
                            Enemybullets[i].position = BossPosition;
                            robotLifePoints--;
                            std::cout << "Robot life points: " << robotLifePoints << std::endl;
                        }
                        else
                        {
                            //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                            res.bulletShader->use();
                            glm::mat4 bulletMatrix = glm::mat4(1.0f);
                            bulletMatrix = glm::translate(bulletMatrix, Enemybullets[i].position + glm::vec3(0.0f, 5.0f, 0.0f));
                            bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                            res.bulletShader->setMat4("model", bulletMatrix);
                            res.bulletShader->setMat4("projection", projection);
                            res.bulletShader->setMat4("view", view);
                            res.bulletModel->Draw(*res.bulletShader);

                        }
                    }
                    else
                    {
                        // Shoot a bullet towards the player
                        Enemybullets[i].position = cannonPositionsUP[i];
                        Enemybullets[i].direction = cannonDirectionsUP[i];
                        Enemybullets[i].active = true;
                    }

                }







            }



            if (hitbox(position, 2.0f, -res.exitRoom2UP, 2.0f))
            {
                roomActive = 0;
                position = -position - glm::vec3(0.0f, 0.0f, 1.0f);


                speed = 45.0f;
                BossPosition = glm::vec3(0.0f, 0.0f, roomSize.z - roomSize.z / 2);
                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
                }

                for (int i = 0; i < Enemybullets.size(); i++)
                {
                    Enemybullets[i].active = false;
                    Enemybullets[i].position = BossPosition;
                }
            }
            
            
            if (hitbox(position, 2.0f, res.exitRoom2UP, 2.0f))
            {
                PowerUp = true;
            }

			if (!PowerUp)
            {
                res.gemShader->use();
                glm::mat4 gem = glm::mat4(1.0f);
                glm::vec3 gemPos = (res.exitRoom2UP) - glm::vec3(0.0f, 2.0f, 2.0f);
                gem = glm::translate(gem, gemPos);
                gem = glm::scale(gem, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
                res.gemShader->setMat4("model", gem);
                res.gemShader->setMat4("view", view);
                res.gemShader->setMat4("projection", projection);
                res.gemShader->setVec3("light.position", lightPos);
                res.gemShader->setVec3("viewPos", camera.Position);
                res.gemShader->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
                res.gemShader->setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
                res.gemShader->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

                res.gemShader->setFloat("material.shininess", 14.0f);
                res.gemModel->Draw(*res.gemShader);
            }

        }

        else if (roomActive == 1)
        {
            speed = 45.0f;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(robotAngle), dir);
            res.ourShader->setMat4("model", model);
            res.ourModel->Draw(*res.ourShader);

            BulletActive(bullets);
            speed = 10.0f;

            for (int j = 0; j < bullets.size(); j++)
            {
                if (bullets[j].active)
                {   

                    
                    if (hitbox(bullets[j].position, 1.0f, cannonPositions[0], 4.0f) && cannonLives[0] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLives[0]--;
                        if (cannonLives[0] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositions[1], 4.0f) && cannonLives[1] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLives[1]--;
                        if (cannonLives[1] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositions[2], 4.0f) && cannonLives[2] > 0)
                    {
                        bullets[j].active = false;
                        cannonLives[2]--;
                        if (cannonLives[2] <= 0)
                        {
                            enemykilled++;
                        }
                    }
                    else if (hitbox(bullets[j].position, 1.0f, cannonPositions[3], 4.0f) && cannonLives[3] > 0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        cannonLives[3]--;
                        if (cannonLives[3] <= 0)
                        {
                            enemykilled++;
                        }
					}
                    else
                    {
                        //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                        res.bulletShader->use();
                        glm::mat4 bulletMatrix = glm::mat4(1.0f);
                        bulletMatrix = glm::translate(bulletMatrix, bullets[j].position);
                        bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                        res.bulletShader->setMat4("model", bulletMatrix);
                        res.bulletShader->setMat4("projection", projection);
                        res.bulletShader->setMat4("view", view);
                        res.bulletModel->Draw(*res.bulletShader);
                        num_avaiable--;
                    }
                }

            }

           
                // render cannon


                for (int i = 0; i < 4; i++)
                {

                    if (hitbox(position, 2.0f, cannonPositions[i], 8.0f) && cannonLives[i] > 0)
                    {
                        float dx = position.x - cannonPositions[i].x;
                        float dz = position.z - cannonPositions[i].z;
                        float distanceQuad = dx * dx + dz * dz;

                        float minDistance = 10.5f; // Somma dei due raggi

                        // Se la distanza al quadrato è minore del limite consentito, stanno collidendo

                        float distance = std::sqrt(distanceQuad);
                        float overlap = minDistance - distance;

                        // Trova la direzione in cui spingerli (normalizzazione)
                        float nx = dx / distance;
                        float nz = dz / distance;

                        // Sposta il nemico 'i' un po' indietro e il nemico 'j' un po' in avanti
           


                        position.x += nx * (overlap * 0.1f);
                        position.z += nz * (overlap * 0.1f);

                        float radius = 3.3f; // Raggio dell'entità spinta

                        // Limita sull'asse X (considerando che la stanza va da -roomSize.x a +roomSize.x)
                        if (position.x > roomSize.x - radius) {
                            position.x = roomSize.x - radius;
                        }
                        else if (position.x < -roomSize.x + radius) {
                            position.x = -roomSize.x + radius;
                        }

                        // Limita sull'asse Z (considerando che la stanza va da -roomSize.z a +roomSize.z)
                        if (position.z > roomSize.z - radius) {
                            position.z = roomSize.z - radius;
                        }
                        else if (position.z < -roomSize.z + radius) {
                            position.z = -roomSize.z + radius;
                        }

                    }

                   
                    if (cannonLives[i] > 0)
                    {
                        cannonDirections[i] = glm::normalize(position - cannonPositions[i]);
                        cannonAngle = glm::degrees(std::atan2(cannonDirections[i].x, cannonDirections[i].z));
                        res.cannonShader->use();
                        res.cannonShader->setVec3("light.position", lightPos);
                        res.cannonShader->setVec3("viewPos", camera.Position);

                        res.cannonShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
                        res.cannonShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
                        res.cannonShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                        res.cannonShader->setFloat("material.shininess", 80.0f);

                        glm::mat4 cannonModelMatrix = glm::mat4(1.0f);
                        cannonModelMatrix = glm::translate(cannonModelMatrix, cannonPositions[i]); // translate it down so it's at the center of the scene
                        cannonModelMatrix = glm::scale(cannonModelMatrix, glm::vec3(8.0f, 8.0f, 8.0f));// it's a bit too big for our scene, so scale it down
                        cannonModelMatrix = glm::rotate(cannonModelMatrix, glm::radians(cannonAngle - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                   
                        res.cannonShader->setMat4("model", cannonModelMatrix);
                        res.cannonShader->setMat4("view", view);
                        res.cannonShader->setMat4("projection", projection);
                        res.cannonModel->Draw(*res.cannonShader);


                        BulletActive(Enemybullets);
						// cannon shoot
                        if (Enemybullets[i].active)
                        {
                            if (hitbox(Enemybullets[i].position, 1.0f, position, 2.0f))
                            {
                                Enemybullets[i].active = false;
                                Enemybullets[i].position = BossPosition;
                                robotLifePoints--;
                                std::cout << "Robot life points: " << robotLifePoints << std::endl;
                            }
                            else
                            {
                                //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                                res.bulletShader->use();
                                glm::mat4 bulletMatrix = glm::mat4(1.0f);
                                bulletMatrix = glm::translate(bulletMatrix, Enemybullets[i].position+glm::vec3(0.0f, 5.0f, 0.0f));
                                bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                                res.bulletShader->setMat4("model", bulletMatrix);
                                res.bulletShader->setMat4("projection", projection);
                                res.bulletShader->setMat4("view", view);
                                res.bulletModel->Draw(*res.bulletShader);

                            }
                        }
                        else
                        {
                            // Shoot a bullet towards the player
                            Enemybullets[i].position = cannonPositions[i];
                            Enemybullets[i].direction = cannonDirections[i];
							Enemybullets[i].active = true;
                        }

                    }
                    
                    




                }
                
			
            


            

            if (hitbox(position, 2.0f, -res.exitRoom1R, 2.0f))
            {
                roomActive = 0;
                speed = 45.0f;
                position = -position + glm::vec3(1.0f, 0.0f, 0.0f);
                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
				}

                for (int i = 0; i < Enemybullets.size(); i++)
                {
                    Enemybullets[i].active = false;
                    Enemybullets[i].position = BossPosition;
				}

			}
            else if (hitbox(position, 2.0f, res.exitRoom2UP, 2.0f))
            {
                roomActive = 2;
                speed = 45.0f;
                position = -position +glm::vec3(0.0f, 0.0f, 1.0f);;

                
                for (int i = 0; i < bullets.size(); i++)
                {
                    bullets[i].active = false;
                    bullets[i].position = position;
                }

                for (int i = 0; i < Enemybullets.size(); i++)
                {
                    Enemybullets[i].active = false;
                    Enemybullets[i].position = BossPosition;
                }
            }

       
            RenderPorta(-res.exitRoom1R, view, projection, *res.portalShader, *res.portalModel, roomSize);
            RenderPorta(res.exitRoom2UP, view, projection, *res.portalShader, *res.portalModel, roomSize);

        }

        else if (roomActive == 2)
        {
            bool bossCanShoot = true;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
            model = glm::rotate(model, glm::radians(robotAngle), dir);
            res.ourShader->setMat4("model", model);
            res.ourModel->Draw(*res.ourShader);


            BulletActive(bullets);

            for (int j = 0; j < bullets.size(); j++)
            {
                if (bullets[j].active)
                {
                    if (hitbox(bullets[j].position, 1.0f, BossPosition, 8.0f) && BossLifePoints>=0)
                    {
                        bullets[j].active = false;
                        bullets[j].position = position;
                        BossLifePoints--;
                        if (BossLifePoints < 0)
                        {
                            enemykilled++;
                        }
						std::cout << "Boss life points: " << BossLifePoints << std::endl;
                    }
                    else
                    {
                        //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                        res.bulletShader->use();
                        glm::mat4 bulletMatrix = glm::mat4(1.0f);
                        bulletMatrix = glm::translate(bulletMatrix, bullets[j].position);
                        bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                        res.bulletShader->setMat4("model", bulletMatrix);
                        res.bulletShader->setMat4("projection", projection);
                        res.bulletShader->setMat4("view", view);
                        res.bulletModel->Draw(*res.bulletShader);
                        num_avaiable--;
                    }
                }

            }

            







            // render enemy
            if (BossLifePoints>=0)
            {    
                if (hitbox(BossPosition, 6.0f, position, 2.5f))
                {
					robotLifePoints = robotLifePoints - 4;

                }

                enemyChasing(position, BossPosition, BossAngle);
                glm::vec3 bossDirection = glm::normalize(position - BossPosition);
                res.enemyShader->use();
                res.enemyShader->setVec3("light.position", lightPos);
                res.enemyShader->setVec3("viewPos", camera.Position);

                res.enemyShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
                res.enemyShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
                res.enemyShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                res.enemyShader->setFloat("material.shininess", 80.0f);

                glm::mat4 enemyModelMatrix = glm::mat4(1.0f);
                enemyModelMatrix = glm::translate(enemyModelMatrix, BossPosition); // translate it down so it's at the center of the scene
                enemyModelMatrix = glm::scale(enemyModelMatrix, glm::vec3(6.0f, 6.0f, 6.0f));// it's a bit too big for our scene, so scale it down
                enemyModelMatrix = glm::rotate(enemyModelMatrix, glm::radians(BossAngle - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                res.enemyShader->setMat4("model", enemyModelMatrix);
                res.enemyShader->setMat4("view", view);
                res.enemyShader->setMat4("projection", projection);
                res.enemyModel->Draw(*res.enemyShader);

                
                
                    for (auto& b : Enemybullets) {
                        if (b.active) {
                            bossCanShoot = false;
                           
                        }

                    }

                    if (bossCanShoot)
                    {
                        

                        Enemybullets[0].position = BossPosition;
                        bossShoot(Enemybullets[0], rotateY(bossDirection, 45.0f)); // +45°

                        Enemybullets[1].position = BossPosition;
                        bossShoot(Enemybullets[1], rotateY(bossDirection, 22.0f)); // +22°

                        Enemybullets[2].position = BossPosition;
                        bossShoot(Enemybullets[2], rotateY(bossDirection, -22.0f)); // -22°

                        Enemybullets[3].position = BossPosition;
                        
                        bossShoot(Enemybullets[3], rotateY(bossDirection, -45.0f)); // -45°
                    }
                
            }

            else {
				
                BossPosition = glm::vec3(0.0f, 0.0f, roomSize.z-roomSize.z/2);
                
            }
            BulletActive(Enemybullets);

            for (int j = 0; j < Enemybullets.size(); j++)
            {
                if (Enemybullets[j].active)
                {
                    if (hitbox(Enemybullets[j].position, 1.0f, position, 2.0f))
                    {
                        Enemybullets[j].active = false;
                        Enemybullets[j].position = BossPosition;
                        robotLifePoints--;
                        std::cout << "Robot life points: " << robotLifePoints << std::endl;

                    }
                    else
                    {
                        //std::cout << "Disegno bullet " << j << " posizione: " << bullets[j].position.x << ", " << bullets[j].position.y << ", " << bullets[j].position.z << std::endl;
                        res.bulletShader->use();
                        glm::mat4 bulletMatrix = glm::mat4(1.0f);
                        bulletMatrix = glm::translate(bulletMatrix, Enemybullets[j].position);
                        bulletMatrix = glm::scale(bulletMatrix, glm::vec3(2.0f, 2.0f, 2.0f)); // prova a cambiare la scala
                        res.bulletShader->setMat4("model", bulletMatrix);
                        res.bulletShader->setMat4("projection", projection);
                        res.bulletShader->setMat4("view", view);
                        res.bulletModel->Draw(*res.bulletShader);

                    }
                }
            }




            if (BossLifePoints <= 0)
            {
                if (hitbox(position, 2.0f, -res.exitRoom2UP, 2.0f))
                {
                    roomActive = 1;
                    position = -position - glm::vec3(0.0f, 0.0f, 1.0f);



                    BossPosition = glm::vec3(0.0f, 0.0f, roomSize.z - roomSize.z / 2);
                    for (int i = 0; i < bullets.size(); i++)
                    {
                        bullets[i].active = false;
                        bullets[i].position = position;
                    }

                    for (int i = 0; i < Enemybullets.size(); i++)
                    {
                        Enemybullets[i].active = false;
                        Enemybullets[i].position = BossPosition;
                    }
                }
            }

            RenderPorta(-res.exitRoom2UP, view, projection, *res.portalShader, *res.portalModel, roomSize);

            
          
		}

       

        // Imposta shader del cubo luce
        //lightCubeShader.use();
        //glm::mat4 lightModel = glm::mat4(1.0f);
        //lightModel = glm::translate(lightModel, lightPos); // lightPos = posizione della luce
        //lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // piccolo cubo

        //lightCubeShader.setMat4("model", lightModel);
        //lightCubeShader.setMat4("view", view);
        //lightCubeShader.setMat4("projection", projection);

        
        
        
        
        

}
