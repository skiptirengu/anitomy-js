var anitomy = require('./build/Release/anitomy');

// console.log(anitomy.parseSync("[Ouroboros]_Fullmetal_Alchemist_Brotherhood_-_01.mkv"));
// console.log(anitomy.parseSync([
//     "[Ouroboros]_Fullmetal_Alchemist_Brotherhood_-_01.mkv"
// ]));  

anitomy.parseAsync(["Mobile_Suit_Gundam_00_Season_2_Ep07_A_Reunion_and_a_Parting_[1080p,BluRay,x264]_-_THORA.mkv"], function (params) {
    console.log(params);
});