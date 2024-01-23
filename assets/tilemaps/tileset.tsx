<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.10.2" name="spritesheet" tilewidth="32" tileheight="32" tilecount="12" columns="2">
 <image source="../spritesheet.png" width="64" height="192"/>
 <tile id="0" type="block.aseprite">
  <properties>
   <property name="breakable" type="bool" value="false"/>
   <property name="walkable" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="2" type="ground-tile.aseprite">
  <properties>
   <property name="breakable" type="bool" value="false"/>
   <property name="walkable" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="4" type="ice-block.aseprite">
  <properties>
   <property name="breakable" type="bool" value="true"/>
   <property name="walkable" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="6" type="player-down-walk 0.aseprite"/>
 <tile id="7" type="player-down-walk 1.aseprite"/>
 <tile id="8" type="player-down.aseprite"/>
 <tile id="10" type="water.aseprite">
  <properties>
   <property name="breakable" type="bool" value="false"/>
   <property name="walkable" type="bool" value="false"/>
  </properties>
 </tile>
</tileset>
