/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

var expect = require('chai').expect;
var anitomy = require('../build/Release/anitomy-js');

describe('anitomy-js', function () {

    var fixture = require('./data.json');
    var fixtureValues = [];
    var fixtureKeys = Object.keys(fixture);
    for (var key in fixture) fixtureValues.push(fixture[key]);

    describe('anitomy parseSync', function () {
        it('should parse all anime filenames, one by one', function () {
            fixtureKeys.forEach(function (key) {
                expect(anitomy.parseSync(key)).to.deep.equal(fixture[key]);
            });
        });

        it('should parse all anime filenames, all at once', function () {
            expect(anitomy.parseSync(fixtureKeys)).to.deep.equal(fixtureValues);
        });

        it('should parse anime filenames, ignoring episode number, title, file extension and release group', function () {
            var parsed = anitomy.parseSync(fixtureKeys, {
                "parse_episode_title": false,
                "parse_episode_number": false,
                "parse_file_extension": false,
                "parse_release_group": false
            });
            parsed.forEach(function (anime) {
                expect(anime).to.not.have.property("episode_title");
                // check episode_number_alt because anitomy still parsing
                // some file's episode number even with parse_episode_number set to false
                expect(anime).to.not.have.property("episode_number_alt");
                expect(anime).to.not.have.property("file_extension");
                expect(anime).to.not.have.property("release_group");
            });
        });

        it('should ignore string', function () {
            expect(anitomy.parseSync('[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv', {
                "ignored_strings": ['!']
            }).file_name).to.eq('[ANBU]_Princess_Lover_-_01_[2048A39A]');
        });

        it('should override default delimiters', function () {
            expect(anitomy.parseSync("[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director's Cut) [988DB090].mkv", {
                "allowed_delimiters": '_.&+,|'
            })).to.deep.equal({
                anime_title: "Seikon no Qwaser - 13v0 (Uncensored Director's Cut)",
                file_checksum: '988DB090',
                file_extension: 'mkv',
                file_name: '[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut) [988DB090]',
                release_group: 'chibi-Doki'
            });
        });

        it('should throw an exception for wrong datatypes (files)', function () {
            expect(function () { anitomy.parseSync(1); }).to.throw('Wrong data type');
        });

        it('should throw an exception for wrong datatypes (options)', function () {
            expect(function () { anitomy.parseSync("", null); }).to.throw('Options must be an object');
        });

        it('should throw an exception for wrong datatypes (options/ignored_strings)', function () {
            expect(function () {
                anitomy.parseSync("", { "ignored_strings": null });
            }).to.throw('ignored_strings must be an array');
        });

        it('should throw wrong number of arguments', function () {
            expect(function () { anitomy.parseSync(); }).to.throw('Wrong number of arguments');
        });
    });

    describe('anitomy parseAsync', function () {
        it('should parse all anime filenames, one by one', function (done) {
            var length = fixtureKeys.length;
            fixtureKeys.forEach(function (key) {
                anitomy.parseAsync(key, function (data) {
                    expect(data).to.deep.equal(fixture[key]);
                    if (--length == 0) done();
                });
            });
        });

        it('should parse all anime filenames, all at once', function (done) {
            anitomy.parseAsync(fixtureKeys, function (data) {
                expect(data).to.deep.equal(fixtureValues);
                done();
            });
        });

        it('should parse anime filenames, ignoring episode number, title, file extension and release group', function (done) {
            var options = {
                "parse_episode_title": false,
                "parse_episode_number": false,
                "parse_file_extension": false,
                "parse_release_group": false
            };
            anitomy.parseAsync(fixtureKeys, function (data) {
                data.forEach(function (anime) {
                    expect(anime, anime.file_name).to.not.have.property("episode_title");
                    // check episode_number_alt because anitomy still parsing
                    // some file's episode number even with parse_episode_number set to false
                    expect(anime, anime.file_name).to.not.have.property("episode_number_alt");
                    expect(anime, anime.file_name).to.not.have.property("file_extension");
                    expect(anime, anime.file_name).to.not.have.property("release_group");
                });
                done();
            }, options);
        });

        it('should ignore string', function (done) {
            expect(anitomy.parseAsync('[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv', function (data) {
                expect(data.file_name).to.eq('[ANBU]_Princess_Lover_-_01_[2048A39A]');
                done();
            }, { "ignored_strings": ['!'] }));
        });

        it('should override default delimiters', function (done) {
            expect(anitomy.parseAsync("[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director's Cut) [988DB090].mkv",
                function (data) {
                    expect(data).to.deep.equal({
                        anime_title: "Seikon no Qwaser - 13v0 (Uncensored Director's Cut)",
                        file_checksum: '988DB090',
                        file_extension: 'mkv',
                        file_name: '[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut) [988DB090]',
                        release_group: 'chibi-Doki'
                    });
                    done();
                }, { "allowed_delimiters": '_.&+,|' }));
        });

        it('should throw an exception for wrong datatypes (files)', function () {
            expect(function () { anitomy.parseAsync(1, function () { }); }).to.throw('Wrong data type');
        });

        it('should throw an exception for wrong datatypes (callback)', function () {
            expect(function () { anitomy.parseAsync("", null); }).to.throw('Second parameter must be a callback');
        });

        it('should throw an exception for wrong datatypes (options)', function () {
            expect(function () { anitomy.parseAsync("", function () { }, null); }).to.throw('Options must be an object');
        });

        it('should throw an exception for wrong datatypes (options/ignored_strings)', function () {
            expect(function () {
                anitomy.parseAsync("", function () { }, { "ignored_strings": null });
            }).to.throw('ignored_strings must be an array');
        });

        it('should throw wrong number of arguments', function () {
            expect(function () { anitomy.parseAsync(); }).to.throw('Wrong number of arguments');
        });
    });
});
