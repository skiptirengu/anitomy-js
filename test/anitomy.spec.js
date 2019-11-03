'use strict'

var chai = require('chai')
var expect = chai.expect
var anitomy = require('../index.js')

chai.use(require('chai-as-promised'))

describe('anitomy-js', function () {
  var fixture = require('./data.json')
  var fixtureValues = []
  var fixtureKeys = Object.keys(fixture)

  for (var key in fixture) {
    fixtureValues.push(fixture[ key ])
  }

  describe('parseSync', function () {
    it('should parse all anime filenames, one by one', function () {
      fixtureKeys.forEach(function (key) {
        expect(anitomy.parseSync(key)).to.deep.equal(fixture[ key ])
      })
    })

    it('should parse all anime filenames, all at once', function () {
      expect(anitomy.parseSync(fixtureKeys)).to.deep.equal(fixtureValues)
    })

    it('should parse anime filenames, ignoring episode number, title, file extension and release group',
      function () {
        var parsed = anitomy.parseSync(fixtureKeys, {
          'parse_episode_title': false,
          'parse_episode_number': false,
          'parse_file_extension': false,
          'parse_release_group': false
        })
        parsed.forEach(function (anime) {
          expect(anime).to.not.have.property('episode_title')
          // check episode_number_alt because anitomy still parsing
          // some file's episode number even with parse_episode_number set to
          // false
          expect(anime).to.not.have.property('episode_number_alt')
          expect(anime).to.not.have.property('file_extension')
          expect(anime).to.not.have.property('release_group')
        })
      })

    it('should ignore string', function () {
      var parsed = anitomy.parseSync('[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv', { 'ignored_strings': [ '!' ] })
      expect(parsed.file_name).to.eq('[ANBU]_Princess_Lover_-_01_[2048A39A]')
    })

    it('should override default delimiters', function () {
      var parsed = anitomy.parseSync(
        '[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut) [988DB090].mkv',
        { 'allowed_delimiters': '_.&+,|' }
      )

      expect(parsed).to.deep.equal({
        anime_title: 'Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut)',
        file_checksum: '988DB090',
        file_extension: 'mkv',
        file_name:
          '[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut) [988DB090]',
        release_group: 'chibi-Doki'
      })
    })

    it('should throw an exception for wrong datatypes (files)', function () {
      expect(function () {
        anitomy.parseSync(1)
      }).to.throw('Wrong data type')
    })

    it('should use empty object as default options', function () {
      expect(anitomy.parseSync('', null)).to.be.deep.equals({})
    })

    it('should throw an exception for wrong datatypes (options/ignored_strings)', function () {
      expect(function () {
        anitomy.parseSync('', { 'ignored_strings': null })
      }).to.throw('ignored_strings must be an array')
    })

    it('should throw wrong number of arguments', function () {
      expect(function () {
        anitomy.parseSync()
      }).to.throw('Wrong data type')
    })
  })

  describe('parseAsync', function () {
    it('should parse all anime filenames, one by one', function () {
      var length = fixtureKeys.length
      var promises = fixtureKeys.map(function (key) {
        return anitomy.parseAsync(key).then(function (data) {
          expect(data).to.deep.equal(fixture[ key ])
          --length
        })
      })
      expect(promises).to.length(fixtureKeys.length)
      return Promise.all(promises).then(function () {
        expect(length).to.be.equals(0)
      })
    })

    it('should parse all anime filenames, all at once', function () {
      return expect(anitomy.parseAsync(fixtureKeys)).eventually.deep.equals(fixtureValues)
    })

    it('should parse anime filenames, ignoring episode number, title, file extension and release group', function () {
      var options = {
        'parse_episode_title': false,
        'parse_episode_number': false,
        'parse_file_extension': false,
        'parse_release_group': false
      }
      return anitomy.parseAsync(fixtureKeys, options).then(function (data) {
        data.forEach(function (anime) {
          // check episode_number_alt because anitomy still parsing
          // some file's episode number even with parse_episode_number set to
          // false
          expect(anime, anime.file_name).to.not.have.property('episode_title')
          expect(anime, anime.file_name).to.not.have.property('episode_number_alt')
          expect(anime, anime.file_name).to.not.have.property('file_extension')
          expect(anime, anime.file_name).to.not.have.property('release_group')
        })
      })
    })

    it('should ignore string', function () {
      var options = { 'ignored_strings': [ '!' ] }
      var input = '[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv'
      return expect(anitomy.parseAsync(input, options)).eventually.include({
        file_name: '[ANBU]_Princess_Lover_-_01_[2048A39A]'
      })
    })

    it('should override default delimiters', function () {
      var input = '[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut) [988DB090].mkv'
      var options = { 'allowed_delimiters': '_.&+,|' }
      return expect(anitomy.parseAsync(input, options)).eventually.deep.equals({
        anime_title: 'Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut)',
        file_checksum: '988DB090',
        file_extension: 'mkv',
        file_name: '[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director\'s Cut) [988DB090]',
        release_group: 'chibi-Doki'
      })
    })

    it('should throw an exception for wrong datatypes (files)', function () {
      return expect(anitomy.parseAsync(1)).be.rejectedWith('Wrong data type')
    })

    it('should not throw and return empty object', function () {
      return expect(anitomy.parseAsync('', null, {})).to.eventually.be.deep.equals({})
    })

    it('should throw an exception for wrong datatypes (options)', function () {
      return expect(anitomy.parseAsync('', null)).to.eventually.be.rejectedWith('Options must be an object')
    })

    it('should throw an exception for wrong datatypes (options.ignored_strings)', function () {
      var options = { 'ignored_strings': null }
      return expect(anitomy.parseAsync('', options)).to.eventually.be.rejectedWith('ignored_strings must be an array')
    })

    it('should throw an exception for wrong datatypes (input)', function () {
      return expect(anitomy.parseAsync()).to.eventually.be.rejectedWith('Wrong data type')
    })

    it('should emit deprecation warning on single argument callback', function (done) {
      // warning event was only implemented on node > 6
      var nodeVersion = Number(process.version.match(/^v(\d+\.\d+)/)[ 1 ])
      if (nodeVersion < 6) {
        this.skip()
      } else {
        var originalErr = console.error
        console.error = function () {
          // prevent output pollution
        }
        process.on('warning', function (err) {
          console.error = originalErr
          if (nodeVersion < 8) {
            expect(err.message).to.be.equals('Using a callback with only one argument is deprecated. Switch to using a node style callback (err, data) or use the promise api.')
          } else {
            expect(err.code).to.be.equals('anitomy-js#parseAsync')
            expect(err.name).to.be.equals('DeprecationWarning')
            expect(err.message).to.be.equal('Using a callback with only one argument is deprecated. Switch to using a node style callback (err, data) or use the promise api.')
          }
          done()
        })
        anitomy.parseAsync('', function (data) {
          //
        })
      }
    })

    it('should accept callback with two arguments with data as 2nd argument', function (done) {
      process.on('warning', function (dep) {
        expect(dep.code).to.not.be.equals('anitomy-js#parseAsync')
      })
      anitomy.parseAsync('', function (err, data) {
        expect(err).to.be.undefined
        expect(data).to.be.deep.equals({})
        done()
      })
    })

    it('should accept callback with two arguments with error as 1st argument', function (done) {
      anitomy.parseAsync(undefined, function (err, data) {
        expect(err).to.be.an.instanceOf(Error)
        expect(data).to.be.undefined
        done()
      })
    })
  })
})
