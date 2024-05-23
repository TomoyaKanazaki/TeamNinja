xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 12;
 2.41737;0.45932;-3.93533;,
 2.09135;-2.54848;-3.08604;,
 2.61173;-1.34955;-4.44162;,
 0.91865;0.97331;-0.03103;,
 -0.42321;-1.44431;3.46470;,
 -0.57531;-5.59773;3.86088;,
 2.18274;-9.86301;-3.32404;,
 0.34355;-10.43726;1.46724;,
 2.87561;-7.37006;-5.12899;,
 1.30182;-0.87102;-4.34428;,
 -2.88583;-2.28946;-2.29926;,
 -1.75646;-8.43854;-4.18930;;
 
 20;
 3;0,1,2;,
 3;3,1,0;,
 3;4,1,3;,
 3;5,1,4;,
 3;6,5,7;,
 3;5,8,1;,
 3;5,6,8;,
 3;9,1,10;,
 3;9,2,1;,
 3;9,3,0;,
 3;9,10,3;,
 3;7,11,6;,
 3;11,8,6;,
 3;7,10,11;,
 3;7,5,10;,
 3;11,1,8;,
 3;11,10,1;,
 3;5,4,10;,
 3;4,3,10;,
 3;9,0,2;;
 
 MeshMaterialList {
  3;
  20;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.097255;0.150588;0.279216;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.008000;0.008000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  28;
  0.933578;-0.000002;0.358375;,
  0.933576;-0.000000;0.358380;,
  0.933580;0.000001;0.358369;,
  0.933581;0.000000;0.358366;,
  0.933582;-0.000001;0.358363;,
  0.933581;0.000002;0.358368;,
  0.933580;0.000002;0.358369;,
  0.933581;0.000005;0.358367;,
  0.933580;0.000003;0.358370;,
  -0.157055;-0.386039;-0.909015;,
  -0.163289;-0.208800;-0.964230;,
  -0.171583;-0.812716;-0.556823;,
  -0.217133;-0.383996;-0.897441;,
  -0.291966;-0.658037;-0.694077;,
  -0.566557;0.738273;-0.366014;,
  -0.637494;0.748361;-0.183187;,
  -0.665319;0.663251;-0.342708;,
  -0.614204;0.783231;-0.096454;,
  -0.288031;-0.931484;-0.222204;,
  -0.166572;-0.136685;-0.976510;,
  -0.936425;-0.264674;0.230337;,
  -0.945660;-0.155350;0.285646;,
  -0.933108;-0.250637;0.257858;,
  -0.948419;-0.106160;0.298717;,
  -0.199833;0.304962;-0.931163;,
  -0.261695;0.340141;-0.903228;,
  -0.862883;0.399049;0.310151;,
  0.027974;0.272217;-0.961829;;
  20;
  3;0,2,1;,
  3;3,2,0;,
  3;4,2,3;,
  3;5,2,4;,
  3;6,5,7;,
  3;5,8,2;,
  3;5,6,8;,
  3;9,12,10;,
  3;9,13,12;,
  3;14,15,16;,
  3;14,17,15;,
  3;18,11,11;,
  3;11,19,11;,
  3;20,21,22;,
  3;20,23,21;,
  3;24,25,19;,
  3;24,10,12;,
  3;23,26,21;,
  3;26,15,17;,
  3;9,27,27;;
 }
 MeshTextureCoords {
  12;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
