/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

// TODO add tests for ignored_strings and allowed_delimiters
var expect = require('chai').expect;
var anitomy = require('../build/Release/anitomy-js');

describe('anitomy-js', function () {

    var fixture = require('./data.json');
    var fixtureValues = [];
    var fixtureKeys = Object.keys(fixture);
    for (var key in fixture) fixtureValues.push(fixture[key]);

    describe('anitomy parseSync', function () {
        it('should parse all anime filenames synchronously, one by one', function () {
            fixtureKeys.forEach(function (key) {
                expect(anitomy.parseSync(key)).to.deep.equal(fixture[key]);
            });
        });

        it('should parse all anime filenames synchronously, all at once', function () {
            expect(anitomy.parseSync(fixtureKeys)).to.deep.equal(fixtureValues);
        });

        it('should parse anime filenames sync ignoring episode number, title, file extension and release group', function () {
            var parsed = anitomy.parseSync(fixtureKeys, {
                "parse_episode_title": false,
                "parse_episode_number": false,
                "parse_file_extension": false,
                "parse_release_group": false
            });
            parsed.forEach(function (anime) {
                expect(anime).to.not.have.property("episode_title");
                expect(anime).to.not.have.property("episode_number_alt");
                expect(anime).to.not.have.property("file_extension");
                expect(anime).to.not.have.property("release_group");
            });
        });

        it('should throw an exception for wrong datatypes (files)', function () {
            expect(function () { anitomy.parseSync(1); }).to.throw('Wrong data type');
        });

        it('should throw an exception for wrong datatypes (options)', function () {
            expect(function () { anitomy.parseSync("", null); }).to.throw('Options must be an object');
        });
    });

    describe('anitomy parseAsync', function () {
        it('should parse all anime filenames asynchronously, one by one', function (done) {
            var length = fixtureKeys.length;
            fixtureKeys.forEach(function (key) {
                anitomy.parseAsync(key, function (data) {
                    expect(data).to.deep.equal(fixture[key]);
                    if (--length == 0) done();
                });
            });
        });

        it('should parse all anime filenames asynchronously, all at once', function (done) {
            anitomy.parseAsync(fixtureKeys, function (data) {
                expect(data).to.deep.equal(fixtureValues);
                done();
            });
        });

        it('should parse anime filenames async ignoring episode number, title, file extension and release group', function (done) {
            var options = {
                "parse_episode_title": false,
                "parse_episode_number": false,
                "parse_file_extension": false,
                "parse_release_group": false
            };
            anitomy.parseAsync(fixtureKeys, function (data) {
                data.forEach(function (anime) {
                    expect(anime, anime.file_name).to.not.have.property("episode_title");
                    expect(anime, anime.file_name).to.not.have.property("episode_number_alt");
                    expect(anime, anime.file_name).to.not.have.property("episode_number");
                    expect(anime, anime.file_name).to.not.have.property("file_extension");
                    expect(anime, anime.file_name).to.not.have.property("release_group");
                });
                done();
            }, options);
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
    });
});