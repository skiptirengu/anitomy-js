const anitomy = require('../index.js')

describe('anitomy-js', () => {
  const fixture = require('./data.json')
  const fixtureKeys = Object.keys(fixture)
  const fixtureValues = fixtureKeys.map((key) => fixture[key])

  // ========= parseSync tests =========
  describe('parseSync', () => {
    it.each(fixtureKeys)('should parse all anime filenames, one by one %s', (key) => {
      const result = anitomy.parseSync(key)

      expect(result).toEqual(fixture[key], `key: ${key}`)
    })

    it('should parse all anime filenames, all at once', () => {
      expect(anitomy.parseSync(fixtureKeys)).toEqual(fixtureValues)
    })

    it('should parse anime filenames, ignoring episode number, title, file extension and release group', () => {
      const parsed = anitomy.parseSync(fixtureKeys, {
        parse_episode_title: false,
        parse_episode_number: false,
        parse_file_extension: false,
        parse_release_group: false,
      })

      expect(parsed.length).toEqual(fixtureKeys.length)
      parsed.forEach((anime) => {
        expect(anime).not.toHaveProperty('episode_title')
        // check episode_number_alt because anitomy still parsing
        // some file's episode number even with parse_episode_number set to
        // false
        expect(anime).not.toHaveProperty('episode_number_alt')
        expect(anime).not.toHaveProperty('file_extension')
        expect(anime).not.toHaveProperty('release_group')
      })
    })

    it('should ignore string', () => {
      const parsed = anitomy.parseSync('[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv', {
        ignored_strings: ['!'],
      })

      expect(parsed.file_name).toEqual('[ANBU]_Princess_Lover_-_01_[2048A39A]')
    })

    it('should override default delimiters', () => {
      const parsed = anitomy.parseSync(
        "[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director's Cut) [988DB090].mkv",
        { allowed_delimiters: '_.&+,|' },
      )

      expect(parsed).toEqual({
        anime_title: "Seikon no Qwaser - 13v0 (Uncensored Director's Cut)",
        file_checksum: '988DB090',
        file_extension: 'mkv',
        file_name: "[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director's Cut) [988DB090]",
        release_group: 'chibi-Doki',
      })
    })

    it('should throw an exception for wrong datatypes (files)', () => {
      expect(() => anitomy.parseSync(1)).toThrow('Input must be either a string or array')
    })

    it('should use empty object as default options', () => {
      expect(anitomy.parseSync('', null)).toEqual({})
    })

    it('should throw wrong number of arguments', () => {
      expect(() => anitomy.parseSync()).toThrow('Input must be either a string or array')
    })
  })

  // ========= parseAsync tests =========
  describe('parseAsync', () => {
    it.each(fixtureKeys)('should parse all anime filenames, one by one %s', async (key) => {
      const result = await anitomy.parseAsync(key)

      expect(result).toEqual(fixture[key], `key: ${key}`)
    })

    it('should parse all anime filenames, all at once', async () => {
      const result = await anitomy.parseAsync(fixtureKeys)

      return expect(result).toEqual(fixtureValues)
    })

    it('should parse anime filenames, ignoring episode number, title, file extension and release group', async () => {
      const options = {
        parse_episode_title: false,
        parse_episode_number: false,
        parse_file_extension: false,
        parse_release_group: false,
      }

      const parsed = await anitomy.parseAsync(fixtureKeys, options)

      expect(parsed.length).toEqual(fixtureKeys.length)
      parsed.forEach(function (anime) {
        expect(anime).not.toHaveProperty('episode_title')
        // check episode_number_alt because anitomy still parsing
        // some file's episode number even with parse_episode_number set to
        // false
        expect(anime).not.toHaveProperty('episode_number_alt')
        expect(anime).not.toHaveProperty('file_extension')
        expect(anime).not.toHaveProperty('release_group')
      })
    })

    it('should ignore string', async () => {
      const options = { ignored_strings: ['!'] }
      const input = '[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv'

      const result = await anitomy.parseAsync(input, options)

      return expect(result).toMatchObject({
        file_name: '[ANBU]_Princess_Lover_-_01_[2048A39A]',
      })
    })

    it('should override default delimiters', async () => {
      // eslint-disable-next-line prettier/prettier
      const input = "[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director's Cut) [988DB090].mkv"
      const options = { allowed_delimiters: '_.&+,|' }

      const result = await anitomy.parseAsync(input, options)

      return expect(result).toEqual({
        anime_title: "Seikon no Qwaser - 13v0 (Uncensored Director's Cut)",
        file_checksum: '988DB090',
        file_extension: 'mkv',
        file_name: "[chibi-Doki] Seikon no Qwaser - 13v0 (Uncensored Director's Cut) [988DB090]",
        release_group: 'chibi-Doki',
      })
    })

    it('should throw an exception for wrong datatypes (files)', async () => {
      await expect(anitomy.parseAsync(1)).rejects.toThrow('Input must be either a string or array')
    })

    it('should not throw and return empty object', async () => {
      return expect(anitomy.parseAsync('', null)).resolves.toEqual({})
    })
  })
})
