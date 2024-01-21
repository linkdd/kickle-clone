<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.10.2" name="spritesheet" tilewidth="32" tileheight="32" tilecount="4" columns="1">
 <image source="../spritesheet.png" width="32" height="128"/>
 <tile id="0" type="block.aseprite">
  <properties>
   <property name="breakable" type="bool" value="false"/>
   <property name="walkable" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="1" type="ground-tile.aseprite">
  <properties>
   <property name="breakable" type="bool" value="false"/>
   <property name="walkable" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="2" type="ice-block.aseprite">
  <properties>
   <property name="breakable" type="bool" value="true"/>
   <property name="walkable" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="3" type="water.aseprite">
  <properties>
   <property name="breakable" type="bool" value="false"/>
   <property name="walkable" type="bool" value="false"/>
  </properties>
 </tile>
</tileset>
