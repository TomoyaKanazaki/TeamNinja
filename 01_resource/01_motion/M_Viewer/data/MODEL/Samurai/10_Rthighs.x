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
 5.18634;-27.62501;-0.19081;,
 6.89932;-0.37339;-2.51797;,
 0.60675;0.29296;4.84437;,
 -1.32753;-28.61174;12.24408;,
 -4.76425;0.74183;-0.63017;,
 -11.47227;-28.22687;6.39439;,
 -16.65403;-28.36677;-0.47791;,
 -10.83520;-27.56464;-6.30351;,
 -0.43715;0.09678;-6.37959;,
 -1.38309;-28.50482;-11.89817;,
 6.89932;-0.37339;-2.51797;,
 5.18634;-27.62501;-0.19081;;
 
 6;
 4;0,1,2,3;,
 4;3,2,4,5;,
 3;6,4,7;,
 4;7,4,8,9;,
 3;5,4,6;,
 4;9,8,10,11;;
 
 MeshMaterialList {
  2;
  6;
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
    "data\\TEXTURE\\MODEL\\samurai_Tex_New.png";
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
  8;
  -0.631103;0.235201;-0.739182;,
  0.826628;0.034291;0.561704;,
  -0.665641;0.258855;-0.699940;,
  -0.674510;0.302418;0.673483;,
  -0.697987;0.281658;-0.658395;,
  -0.575124;0.285253;0.766722;,
  -0.761171;0.313879;0.567538;,
  0.694068;-0.019861;-0.719636;;
  6;
  4;1,1,1,1;,
  4;5,5,3,3;,
  3;4,2,2;,
  4;2,2,0,0;,
  3;3,3,6;,
  4;7,7,7,7;;
 }
 MeshTextureCoords {
  12;
  0.506140;0.970170;,
  0.506140;0.678570;,
  0.411450;0.681210;,
  0.329190;0.999740;,
  0.347060;0.642840;,
  0.224530;0.953750;,
  0.112200;0.883550;,
  0.052670;0.773010;,
  0.305200;0.586120;,
  0.000260;0.679080;,
  0.300810;0.505210;,
  0.009230;0.508790;;
 }
}
