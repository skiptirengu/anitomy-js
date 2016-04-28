var assert = require('chai').assert;
var anitomy = require('../build/Release/anitomy-js');

var fixture = "[Juuni.Kokki]-(Les.12.Royaumes)-[Ep.24]-[x264+OGG]-[JAP+FR+Sub.FR]-[Chap]-[AzF].mkv";

describe('anitomy', function () {

    describe('anitomy parseSync', function () {
        it('should parse an anime filename synchronously', function () {
            var parse = anitomy.parseSync(fixture);
            assert.isObject(parse);
        });

        it('should parse an array of anime filenames synchronously', function () {
            var parse = anitomy.parseSync([fixture]);
            assert.isArray(parse);
        })
    });

    describe('anitomy parseAsync', function () {
        it('should parse an anime filename asynchronously', function (done) {
            var parse = anitomy.parseAsync(fixture, function (data) {
                assert.isObject(data);
                done();
            });
            assert.isUndefined(parse);
        });
        
        it('should parse an array of anime filenames asynchronously', function (done) {
            var parse = anitomy.parseAsync([fixture], function (data) {
                assert.isArray(data);
                done();
            });
            assert.isUndefined(parse);
        });
    });
});