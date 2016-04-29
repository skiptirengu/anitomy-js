/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

var assert = require('chai').assert;
var expect = require('chai').expect;

var anitomy = require('../build/Release/anitomy-js');

describe('anitomy', function () {

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

        it('should throw an exception for wrong datatypes', function () {
            expect(function () { anitomy.parseSync(1); }).to.throw('Wrong data type');
        });
    });

    describe('anitomy parseSync', function () {
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

        it('should throw an exception for wrong datatypes', function () {
            expect(function () { anitomy.parseAsync(1, function () { }); }).to.throw('Wrong data type');
        });

        it('should throw an exception for wrong datatypes', function () {
            expect(function () { anitomy.parseAsync(""); }).to.throw('Second parameter must be a callback');
        });
    });

});