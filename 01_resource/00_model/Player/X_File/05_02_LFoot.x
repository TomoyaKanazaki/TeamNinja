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
 18;
 2.31553;9.70087;6.27332;,
 -2.21914;4.42587;-6.45774;,
 3.38657;-0.52413;-9.37950;,
 7.33703;-0.52413;6.31692;,
 -1.59638;-0.52413;9.38636;,
 -7.84743;-0.52413;-3.98874;,
 -6.18734;-0.52413;-10.74834;,
 -2.69557;-0.52413;-12.23050;,
 3.78226;-0.52413;10.80762;,
 2.31553;10.24347;6.27332;,
 3.38657;0.01849;-9.37950;,
 -2.21914;4.96849;-6.45774;,
 7.33703;0.01849;6.31692;,
 -1.59638;0.01849;9.38636;,
 -7.84743;0.01849;-3.98874;,
 -6.18734;0.01849;-10.74834;,
 -2.69557;0.01849;-12.23050;,
 3.78226;0.01849;10.80762;;
 
 18;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,1;,
 3;4,1,0;,
 3;5,6,1;,
 3;6,7,1;,
 3;1,7,2;,
 3;4,0,8;,
 3;0,3,8;,
 3;9,10,11;,
 3;9,12,10;,
 3;13,11,14;,
 3;13,9,11;,
 3;14,11,15;,
 3;15,11,16;,
 3;11,10,16;,
 3;13,17,9;,
 3;9,17,12;;
 
 MeshMaterialList {
  1;
  18;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.000000;0.037600;0.279200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  24;
  -0.793658;-0.606744;0.044373;,
  -0.164319;-0.779679;0.604235;,
  -0.568133;-0.675625;0.469847;,
  -0.879832;-0.432922;-0.196148;,
  0.644510;-0.579410;-0.498890;,
  0.647359;-0.760597;-0.049180;,
  0.501374;-0.760150;0.413274;,
  -0.019000;-0.758219;0.651723;,
  0.237844;-0.365034;-0.900100;,
  0.752945;-0.652101;-0.088535;,
  0.905014;-0.394607;-0.158855;,
  -0.731055;-0.361489;-0.578692;,
  0.793657;0.606745;-0.044372;,
  0.164319;0.779679;-0.604235;,
  0.568133;0.675625;-0.469847;,
  0.879832;0.432923;0.196148;,
  -0.644509;0.579410;0.498890;,
  -0.647359;0.760597;0.049180;,
  -0.501374;0.760151;-0.413274;,
  0.019000;0.758219;-0.651723;,
  -0.237844;0.365035;0.900100;,
  -0.752944;0.652102;0.088535;,
  -0.905014;0.394608;0.158855;,
  0.731055;0.361489;0.578692;;
  18;
  3;0,1,2;,
  3;0,2,3;,
  3;4,5,9;,
  3;4,9,10;,
  3;5,6,9;,
  3;6,7,1;,
  3;1,7,2;,
  3;4,8,8;,
  3;0,3,11;,
  3;12,14,13;,
  3;12,15,14;,
  3;16,21,17;,
  3;16,22,21;,
  3;17,21,18;,
  3;18,13,19;,
  3;13,14,19;,
  3;16,20,20;,
  3;12,23,15;;
 }
 MeshTextureCoords {
  18;
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
