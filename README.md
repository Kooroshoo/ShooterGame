# ShooterGame
 Implementing a Teleport ability for the “Shooter Game” example project that work's properly in multiplayer. 
 
 ![Gif](https://user-images.githubusercontent.com/26629624/109434879-a3732180-7a17-11eb-87e9-cc9a0ddac18c.gif)
 
The player is able to teleport 10 meters forward upon pressing the T key. this is implemented for a multiplayer environment where the integration with the internal character replication system is not trivial. We Make sure to stress this mechanic on multiplayer by simulating a large packet lag. Use a lag simulation of 500ms with 30% lag variance. 

#
Unreal ver. 4.25

Asset: “Shooter Game” example project downloadable for free from the Epic Games Launcher -> Learn.
