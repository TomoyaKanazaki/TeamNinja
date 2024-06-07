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
 24;
 1.31515;-14.20296;-2.35509;,
 3.05683;-0.77224;-2.82199;,
 3.05683;-0.77224;3.27199;,
 1.13413;-17.00222;1.99849;,
 1.13413;-17.00222;1.99849;,
 3.05683;-0.77224;3.27199;,
 -3.03716;-0.77224;3.27199;,
 -1.11447;-17.00222;1.99849;,
 -1.11447;-17.00222;1.99849;,
 -3.03716;-0.77224;3.27199;,
 -3.03716;-0.77224;-2.82199;,
 -1.15315;-14.20296;-2.35509;,
 3.05683;-0.77224;-2.82199;,
 -3.03716;-0.77224;-2.82199;,
 -2.10334;-17.53044;-1.88817;,
 -1.37353;-15.89319;-8.00815;,
 -1.12319;-17.37188;-8.07463;,
 1.39321;-15.89319;-8.00815;,
 1.14285;-17.37188;-8.07463;,
 2.12301;-17.53044;-1.88817;,
 2.12301;-17.53044;-1.88817;,
 -2.10334;-17.53044;-1.88817;,
 -1.12319;-17.37188;-8.07463;,
 1.14285;-17.37188;-8.07463;;
 
 13;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;11,10,1,0;,
 4;6,5,12,13;,
 4;14,11,15,16;,
 4;16,15,17,18;,
 4;11,0,17,15;,
 4;0,19,18,17;,
 4;20,21,22,23;,
 3;11,14,8;,
 3;0,3,19;,
 4;21,20,4,7;;
 
 MeshMaterialList {
  2;
  13;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ENEMY\\SAMURAI\\samurai_Tex_New.png";
   }
  }
  Material {
   0.416000;0.320000;0.064000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.987496;0.149807;0.049079;,
  0.992414;-0.121845;-0.016349;,
  -0.982633;0.179188;0.048213;,
  -0.991550;-0.125776;-0.031777;,
  0.000000;-0.078225;0.996936;,
  0.997909;0.058611;0.027229;,
  0.992853;0.069808;0.096796;,
  -0.996793;0.077091;0.021447;,
  -0.993034;0.056847;-0.103210;,
  0.000000;0.044914;-0.998991;,
  -0.992525;0.087041;0.085539;,
  0.000000;-0.034743;-0.999396;,
  0.000000;1.000000;0.000000;,
  0.000000;0.958091;-0.286465;,
  0.993409;0.035928;-0.108851;,
  0.000000;-0.998503;0.054699;,
  0.000000;-0.999672;-0.025622;,
  0.000000;-0.990891;0.134668;;
  13;
  4;5,1,1,6;,
  4;4,4,4,4;,
  4;10,3,3,7;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;2,7,8,8;,
  4;9,9,9,9;,
  4;13,13,13,13;,
  4;5,0,14,14;,
  4;15,15,16,16;,
  3;7,2,10;,
  3;5,6,0;,
  4;15,15,17,17;;
 }
 MeshTextureCoords {
  24;
  0.836160;0.570900;,
  0.846310;0.656550;,
  0.891610;0.656100;,
  0.891610;0.563360;,
  0.865850;0.385380;,
  0.884610;0.285180;,
  0.825150;0.285180;,
  0.843910;0.385380;,
  0.748730;0.554370;,
  0.744460;0.655350;,
  0.795110;0.656780;,
  0.804830;0.568270;,
  0.884610;0.225720;,
  0.825150;0.225720;,
  0.778650;0.536320;,
  0.817420;0.517060;,
  0.815660;0.507150;,
  0.830850;0.519260;,
  0.834440;0.507750;,
  0.866440;0.544320;,
  0.875500;0.423650;,
  0.834260;0.423650;,
  0.843830;0.484030;,
  0.865940;0.484030;;
 }
}
